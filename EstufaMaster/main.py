import socket
import threading
import tkinter as tk
from tkinter import messagebox

class UDPServerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("UDP Server Configuration")
        
        # Variáveis
        self.server_running = False
        self.udp_port = 12345
        self.dns_name = "raspberrypi"
        self.ip_address = self.get_ip_address()
        
        # Interface gráfica
        self.create_widgets()
        
        # Thread do servidor UDP
        self.udp_thread = None
        
    def create_widgets(self):
        tk.Label(self.root, text="DNS Name:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=5)
        self.dns_entry = tk.Entry(self.root)
        self.dns_entry.grid(row=0, column=1, padx=5, pady=5)
        self.dns_entry.insert(0, self.dns_name)
        
        tk.Label(self.root, text="UDP Port:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=5)
        self.port_entry = tk.Entry(self.root)
        self.port_entry.grid(row=1, column=1, padx=5, pady=5)
        self.port_entry.insert(0, str(self.udp_port))
        
        tk.Label(self.root, text="IP Address:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=5)
        tk.Label(self.root, text=self.ip_address).grid(row=2, column=1, sticky=tk.W, padx=5, pady=5)
        
        self.status_label = tk.Label(self.root, text="Status: Stopped")
        self.status_label.grid(row=3, column=0, columnspan=2, sticky=tk.W, padx=5, pady=5)
        
        tk.Button(self.root, text="Salvar", command=self.save_config).grid(row=4, column=0, padx=5, pady=5)
        tk.Button(self.root, text="Parar", command=self.exit_app).grid(row=4, column=1, padx=5, pady=5)
        tk.Button(self.root, text="Encerrar", command=self.exit_app).grid(row=5, column=1, padx=5, pady=5)
        
        # Start the server
        self.start_server()

    def get_ip_address(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            s.connect(("8.8.8.8", 80))
            ip = s.getsockname()[0]
        finally:
            s.close()
        return ip
    
    def start_server(self):
        if not self.server_running:
            self.server_running = True
            self.status_label.config(text="Status: Running")
            self.udp_thread = threading.Thread(target=self.listen_udp)
            self.udp_thread.daemon = True
            self.udp_thread.start()
        
    def stop_server(self):
        if self.server_running:
            self.server_running = False
            self.status_label.config(text="Status: Stopped")
    
    def listen_udp(self):
        print("Listening for UDP messages...")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((self.ip_address, self.udp_port))
        print(self.ip_address, self.udp_port)
        self.sock.settimeout(1.0)  # Define um timeout para evitar travamentos
        while self.server_running:
            print("Listening for UDP messages...")
            try:
                data, addr = self.sock.recvfrom(1024)
                print(f"Received message from {addr}: {data.decode('utf-8')}")
            except socket.timeout:
                continue
        self.sock.close()

    
    def save_config(self):
        self.stop_server()
        self.dns_name = self.dns_entry.get()
        self.udp_port = int(self.port_entry.get())
        messagebox.showinfo("Configurações", "Configurações salvas com sucesso!")
        self.start_server()
        
    def exit_app(self):
        self.stop_server()
        self.root.quit()

if __name__ == "__main__":
    root = tk.Tk()
    app = UDPServerApp(root)
    root.mainloop()
