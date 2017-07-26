# -*- coding: utf-8 -*-
#######################################################################################################
#    UnB - Universidade de Brasília     
#    Departamento de Ciência da Computação                                                               
#    Teleinformática e Redes 2 - 2017/1                                                               
#    Professor: Jacir Luiz Bordim 
#    Projeto Final
#    Integrantes:
#    ==============================================                                                                    
#     Edgar Fabiano de Souza Filho - 14/0019201
#     Ingrid Santana Lopes - 14/0083065
#     Marcos Paulo Cayres Rosa - 14/0027131
#     Paulo Victor Gonçalves Farias - 13/0144754
#     Rennê Ruan Alves Oliveira - 14/0030930                                                                     
#    ==============================================                                     
#        
#    vitima_ServerDNS.py = Responsável por receber requisições DNS e retornar uma resposta. Esse servidor
#    DNS precisa funcionar sobre o UDP. Ele é o servidor que receberá os ataques de amplificação de DNS.
#    Com o comando:  dig @192.168.0.110 -p 8000 example.com 
#    Podemos receber uma resposta do servidor.
#    Após algum tempo do ataque de amplificação de DNS, o dig para de funcionar.
#    Ele utiliza uma bibilioteca já existente DNSLIB que facilita os processos com o DNS.
#    Fonte: https://gist.github.com/andreif/6069838
#                                                                        
#######################################################################################################
"""

LICENSE http://www.apache.org/licenses/LICENSE-2.0

"""
import datetime
import sys
import time
import threading
import traceback
import SocketServer
from dnslib import *


class DomainName(str):
    def __getattr__(self, item):
        return DomainName(item + '.' + self)


D = DomainName('example.com')
IP = '127.0.0.1'
TTL = 60 * 5
soa_record = SOA(
    mname=D.ns1,  # primary name server
    rname=D.andrei,  # email of the domain administrator
    times=(
        201307231,  # serial number
        60 * 60 * 1,  # refresh
        60 * 60 * 3,  # retry
        60 * 60 * 24,  # expire
        60 * 60 * 1,  # minimum
    )
)
ns_records = [NS(D.ns1), NS(D.ns2)]
records = {
    D: [A(IP), AAAA((0,) * 16), MX(D.mail), soa_record] + ns_records,
    D.ns1: [A(IP)],  # MX and NS records must never point to a CNAME alias (RFC 2181 section 10.3)
    D.ns2: [A(IP)],
    D.mail: [A(IP)],
    D.andrei: [CNAME(D)],
}


def dns_response(data):
    request = DNSRecord.parse(data)

    print request

    reply = DNSRecord(DNSHeader(id=request.header.id, qr=1, aa=1, ra=1), q=request.q)

    qname = request.q.qname
    qn = str(qname)
    qtype = request.q.qtype
    qt = QTYPE[qtype]

    if qn == D or qn.endswith('.' + D):

        for name, rrs in records.iteritems():
            if name == qn:
                for rdata in rrs:
                    rqt = rdata.__class__.__name__
                    if qt in ['*', rqt]:
                        reply.add_answer(RR(rname=qname, rtype=QTYPE[rqt], rclass=1, ttl=TTL, rdata=rdata))

        for rdata in ns_records:
            reply.add_ns(RR(rname=D, rtype=QTYPE.NS, rclass=1, ttl=TTL, rdata=rdata))

        reply.add_ns(RR(rname=D, rtype=QTYPE.SOA, rclass=1, ttl=TTL, rdata=soa_record))

    print "---- Reply:\n", reply

    return reply.pack()


class BaseRequestHandler(SocketServer.BaseRequestHandler):

    def get_data(self):
        raise NotImplementedError

    def send_data(self, data):
        raise NotImplementedError

    def handle(self):
        now = datetime.datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')
        print "\n\n%s request %s (%s %s):" % (self.__class__.__name__[:3], now, self.client_address[0],
                                               self.client_address[1])
        try:
            data = self.get_data()
            print len(data), data.encode('hex')  # repr(data).replace('\\x', '')[1:-1]
            self.send_data(dns_response(data))
        except Exception:
            traceback.print_exc(file=sys.stderr)


class UDPRequestHandler(BaseRequestHandler):

    def get_data(self):
        return self.request[0].strip()

    def send_data(self, data):
        return self.request[1].sendto(data, self.client_address)


if __name__ == '__main__':
    print "Starting nameserver..."

    servers = [
        SocketServer.ThreadingUDPServer(('192.168.0.110', 8000), UDPRequestHandler),
    ]

    for s in servers:
        thread = threading.Thread(target=s.serve_forever)  # that thread will start one more thread for each request
        thread.daemon = True  # exit the server thread when the main thread terminates
        thread.start()
        print "%s server loop running in thread: %s" % (s.RequestHandlerClass.__name__[:3], thread.name)

    try:
        while 1:
            time.sleep(1)
            sys.stderr.flush()
            sys.stdout.flush()

    except KeyboardInterrupt:
        pass
    finally:
        for s in servers:
            s.shutdown()