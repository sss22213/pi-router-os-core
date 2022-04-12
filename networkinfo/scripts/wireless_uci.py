from uci import Uci

class wireless_uci:
    def __init__(self):
        self.u = Uci()
        self.wire_info = self.u.get("wireless")

    def get_wireless_mode(self, radio_num):
        try:
            return self.wire_info["default_radio"+str(radio_num)]["mode"]
        except:
            return "-1"


if __name__ == '__main__':
    wire_uci = wireless_uci()
    print(wire_uci.get_wireless_mode(2))