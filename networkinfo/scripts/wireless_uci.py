from uci import Uci
import os

class wireless_uci:
    def __init__(self):
        self.u = Uci()
        self.wire_info = self.u.get("wireless")

    def get_wireless_mode(self, radio_num):
        try:
            return self.wire_info["default_radio"+str(radio_num)]["mode"]
        except:
            return "-1"

    def set_wireless_connect_to_ap(self, ssid, key, encryption):
        # Change mode to station mode
        self.u.set("wireless", "default_radio2", "network", 'wan')
        self.u.set("wireless", "default_radio2", "mode", 'sta')
        self.u.set("wireless", "default_radio2", "ssid", ssid)
        self.u.set("wireless", "default_radio2", "encryption", encryption)
        self.u.set("wireless", "default_radio2", "key", key)
        self.u.commit("wireless")
        os.system("/etc/init.d/network restart")

    def set_wirless_ap_mode(self, ssid, key, encryption):
        # Change mode to station mode
        self.u.set("wireless", "default_radio2", "network", 'llan')
        self.u.set("wireless", "default_radio2", "mode", 'ap')
        self.u.set("wireless", "default_radio2", "ssid", ssid)
        self.u.set("wireless", "default_radio2", "encryption", encryption)
        self.u.set("wireless", "default_radio2", "key", key)
        self.u.commit("wireless")
        os.system("/etc/init.d/network restart")
    
    def get_wireless_ip_mode(self, radio_num):
        pass

if __name__ == '__main__':
    wire_uci = wireless_uci()
    #print(wire_uci.get_wireless_mode(2))
    print(wire_uci.set_wireless_connect_to_ap('7734_New','1qazXSW2!','psk2'))