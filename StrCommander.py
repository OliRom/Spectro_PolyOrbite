import serial
from serial.tools import list_ports


def find_arduino(search_str=None):
    ports = list_ports.comports()
    
    if search_str is not None:
        for p in ports:
            if search_str in p.description:
                return p
    
    # The Arduino has not been found
    for p in ports:
        print(p)
    
    port = input("\nSelect the number of the port to connect (COM#): ")
    
    for p in ports:
        if p.name == "COM" + port:
            return p
    
    return None


class StrCmd:
    def __init__(self, arduino_name=None):
        self.connect_arduino(arduino_name)
    
    def connect_arduino(self, name=None):
        # Connect the Arduino
        port_name = None
        while port_name is None:
            port_name = find_arduino(name).name
            
        self.conn = serial.Serial(port_name, 9600)
        self.conn.close()
    
    def exec_cmd(self, verb, noun, args=None):
        cmd = str(verb) + " " + str(noun)
        
        if args is None:
            pass
        
        elif isinstance(args, list):
            cmd += " " + "&".join([str(i) for i in args])
        
        else:
            cmd += " " + str(args)
        
        cmd += "\n"
        
        ard = self.conn  # The Arduino connection
        
        # Open the port and send the command
        ard.open()
        ard.write(cmd.encode("utf-8"))
        ard.flush()  # Send the bytes now
        
        # Wait for the response
        res = list()
        while True:
            if ard.in_waiting > 0:
                line = ard.readline().decode("utf-8")[:-2]  # Remove "\r\n"
                
                if "\x04" in line:  # End of transmission
                    break
                
                res.append(line)
                
        ard.close()
        
        return res
        
if __name__ == "__main__":
    cmd = StrCmd("Périphérique")
    
    print(cmd.exec_cmd("call", "get_temp"))
    print(cmd.exec_cmd(2, ""))
