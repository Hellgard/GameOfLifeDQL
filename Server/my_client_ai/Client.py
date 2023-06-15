import socket
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: python client.py <server_ip> <server_port>")
        return

    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    client_socket.connect((server_ip, server_port))
    print("Connected to server:", server_ip, "port:", server_port)
    data = client_socket.recv(1024).decode()
    print("Received from server:", data)
    while True:
        message = input("Enter a message (or '/quit' to quit): ")
        if message == '/quit':
            break

        client_socket.send(message.encode())

        data = client_socket.recv(1024).decode()
        print("Received from server:", data)

    client_socket.close()

if __name__ == '__main__':
    main()
