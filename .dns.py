#python code for DNS:

import socket 
 
def forward_lookup(domain): 
    try: 
        ip = socket.gethostbyname(domain) 
        print(f"IP Address of {domain}: {ip}") 
    except socket.gaierror: 
        print("Host not found.") 
 
def reverse_lookup(ip_address): 
    try: 
        host = socket.gethostbyaddr(ip_address) 
        print(f"Host Name of {ip_address}: {host[0]}") 
    except socket.herror: 
        print("Host not found.")

def main(): 
    print("DNS Lookup Program") 
    print("1. Forward Lookup (Domain → IP)") 
    print("2. Reverse Lookup (IP → Domain)") 
 
    choice = input("Enter your choice (1/2): ") 
 
    if choice == '1': 
        domain = input("Enter Domain Name: ") 
        forward_lookup(domain) 
    elif choice == '2': 
        ip_address = input("Enter IP Address: ") 
        reverse_lookup(ip_address) 
    else: 
        print("Invalid choice. Exiting.") 
 
if __name__ == "__main__": 
    main() 