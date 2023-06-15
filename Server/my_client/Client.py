import socket
import sys
import threading

def wait_for_switch():
    global command_mode
    while True:
        key = input()
        if key == 's':
            command_mode = not command_mode

def send_command(socket, command):
    socket.send(command.encode())
    response = socket.recv(1024).decode()
    return response

def help_command(socket):
    help_message = """Available commands:
Connection commands:
    /help: display this help
    /quit: quit the program
    /switch: switch between command and AI mode
Gameplay commands:
    /Look: look around
    /Inventory: get inventory
    /Broadcast: broadcast a message
    /Connect_nbr: display number of remaining connections
    /Forward: move forward
    /Right: move right
    /Left: move left
    /Fork: fork a new player
    /Take: take an object from the tile
    /Incantation: start an incantation"""
    print(help_message)

def forward_command(socket):
    response = send_command(socket, "/Forward")
    print("Received from server:", response)
    return response

def right_command(socket):
    response = send_command(socket, "/Right")
    print("Received from server:", response)
    return response

def left_command(socket):
    response = send_command(socket, "/Left")
    print("Received from server:", response)
    return response

def look_command(socket):
    response = send_command(socket, "/Look")
    print("Received from server:", response)
    return response

def inventory_command(socket):
    response = send_command(socket, "/Inventory")
    print("Received from server:", response)
    return response

def broadcast_command(socket):
    message = input("Enter a broadcast message: ")
    response = send_command(socket, "/Broadcast " + message)
    print("Received from server:", response)
    return response

def connect_nbr_command(socket):
    response = send_command(socket, "/Connect_nbr")
    print("Received from server:", response)
    return response

def fork_command(socket):
    response = send_command(socket, "/Fork")
    print("Received from server:", response)
    return response

def take_command(socket):
    item = input("Enter the item name to take: ")
    response = send_command(socket, "/Take " + item)
    print("Received from server:", response)
    return response

def incantation_command(socket):
    response = send_command(socket, "/Incantation")
    print("Received from server:", response)
    return response

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
    command_mode = True

    command_functions = {
        "/help": help_command,
        "/Forward": forward_command,
        "/Right": right_command,
        "/Left": left_command,
        "/Look": look_command,
        "/Inventory": inventory_command,
        "/Broadcast": broadcast_command,
        "/Connect_nbr": connect_nbr_command,
        "/Fork": fork_command,
        "/Take": take_command,
        "/Incantation": incantation_command
    }

    while True:
        if command_mode:
            message = input("Enter a command (/help, '/quit' to quit or /switch to change mode): ")
            if message == '/quit':
                break
            
            if message == '/switch':
                print("Switching to AI mode")
                command_mode = False
                continue
            if message == '/help':
                print("Available commands:")
                print("/help: display this help")
                print("/quit: quit the program")
                print("/login: login to the server")
                print("/password: put your password")
                print("/logout: logout from the server")
                print("/users: display connected users")
                print("/user: display user info")
                print("/send: send a message to a user")
                continue

            client_socket.send(message.encode())

            data = client_socket.recv(1024).decode()
            print("Received from server:", data)
        else:
            message = input("Enter a command (/help, '/quit' to quit or /switch to change mode): ")
            if message == '/quit':
                break
            
            if message == '/switch':
                print("Switching to command mode")
                command_mode = False
                continue
            if message in command_functions:
                data = command_functions[message](client_socket)
            else:
                print("Invalid command. Type '/help' for a list of available commands.")

    client_socket.close()

if __name__ == '__main__':
    main()
