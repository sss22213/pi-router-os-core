import os
import time

class raspi_hw_info:
    def __init__(self):
        pass
    
    def get_cpu_core(self):
        cpu_core_popen = os.popen('cat /proc/cpuinfo | grep processor | wc -l')
        cpu_count = int(cpu_core_popen.read())
        cpu_core_popen.close()
        return cpu_count

    def get_cpu_hardware_name(self):
        cpu_hardware_name_popen = os.popen("cat /proc/cpuinfo | grep Hardware | awk '{print $3}'")
        cpu_hardware_name = cpu_hardware_name_popen.read().split('\n')[0]
        cpu_hardware_name_popen.close()
        return cpu_hardware_name

    def get_model_name(self):
        model_name_popen = os.popen("cat /proc/cpuinfo | grep Model")
        model_name = model_name_popen.read().split(':')[1][1:-1]
        model_name_popen.close()
        return model_name

    def get_linux_infomation(self):
        linux_info_popen = os.popen("uname -a")
        linux_info = linux_info_popen.read()[:-1]
        linux_info_popen.close()
        return linux_info

    def get_pi_sn(self):
        sn_popen = os.popen("cat /proc/cpuinfo | grep Serial")
        sn = sn_popen.read().split(":")[1][1:-1]
        sn_popen.close()
        return sn

    def get_cpu_total_time(self):
        cpu_time_popen = os.popen('cat /proc/stat | grep cpu[^0-9]')
        cpu_time = cpu_time_popen.read().split(" ")[2:-1]
        cpu_time_popen.close()
        cpu_sum_time = 0
        
        for cpu_time_item in cpu_time:
            cpu_sum_time += (int(cpu_time_item))

        return cpu_sum_time

    def get_cpu_total_idle_time(self):
        cpu_time_popen = os.popen('cat /proc/stat | grep cpu[^0-9]')
        cpu_time = cpu_time_popen.read().split(" ")[2:-1]
        cpu_time_popen.close()
        return int(cpu_time[3])

    def get_cpu_usage_percent(self):
        cpu_count = self.get_cpu_core()

        cpu_usage_percent = 0

        total_time1 = self.get_cpu_total_time()
        cpu_idle_time1 = self.get_cpu_total_idle_time()
        time.sleep(0.01)
        total_time2 = self.get_cpu_total_time()
        cpu_idle_time2 = self.get_cpu_total_idle_time()

        total_time_diff = total_time2 - total_time1
        cpu_idle_time_diff = cpu_idle_time2 - cpu_idle_time1

        cpu_usage_percent = round(cpu_idle_time_diff / total_time_diff,2)

        if cpu_usage_percent > 1.0:
            cpu_usage_percent = 1.0

        cpu_usage_percent = 1.0 - cpu_usage_percent
 
        return int(cpu_usage_percent*100)


    def get_pi_router_os_version(self):
        return "dev"

if __name__ == '__main__':
    raspi_hw = raspi_hw_info()
    print(raspi_hw.get_cpu_usage_percent())