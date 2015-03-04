import pygame
import socket
import argparse


UDP_IP = "192.168.1.4"
UDP_PORT = 9899


class Socket(object):
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.sock = socket.socket(socket.AF_INET,       # Internet
                                  socket.SOCK_DGRAM)    # UDP

    def send(self, msg):
        print msg
        self.sock.sendto(msg, (self.ip, self.port))

    def login(self):
        self.send("LOGIN\0")

    def key_down(self, key):
        self.send("PRESSED " + chr(key).upper())

    def key_up(self, key):
        self.send("UNPRESSED " + chr(key).upper())


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('server_ip', nargs='?', default='192.168.1.4')
    parser.add_argument('port', default=9899, nargs='?', type=int)
    args = parser.parse_args()
    c = Socket(args.server_ip, args.port)

    c.login()
    screen = pygame.display.set_mode((400, 300))
    pygame.init()
    finished = False
    while not finished:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN or event.type == pygame.KEYUP:
                if event.key == pygame.K_ESCAPE:
                    finished = True
                    break
                if event.key < 256:
                    if event.type == pygame.KEYDOWN:
                        c.key_down(event.key)
                    else:
                        c.key_up(event.key)
                else:
                    print "Key ignored", event.key
    c.send("LOGOUT\0")


if __name__ == '__main__':
    main()
