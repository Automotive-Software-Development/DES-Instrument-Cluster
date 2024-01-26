import can
from piracer.vehicles import PiRacerStandard
from pydbus import SessionBus
from gi.repository import GLib
import concurrent.futures

global BatteryLevel, Rpm

#########Battery Level in Percent ######################################################################
def BatteryLevelinPer(x): #Calculate Voltage to Percent
    return -25.6269 * x**3 + 887.9706 * x**2 - 10180.5203 * x + 38661.8396

class RollingAverage: #Averaging the last 5 Voltages
    def __init__(self, window_size=5):
        self.window_size = window_size
        self.measurements = []

    def add_measurement(self, value):
        self.measurements.append(value)
        if len(self.measurements) > self.window_size:
            self.measurements = self.measurements[-self.window_size:]

    def get_average(self):
        if not self.measurements:
            return None  # Return None if no measurements are available
        return sum(self.measurements) / len(self.measurements)

def getBatteryLevel():
    rolling_average = RollingAverage()
    #The following lines are important to get Voltage, Average and Transform into Percent: From battery_voltage to batteryLevel
    battery_voltage = piracer.get_battery_voltage() #important to get Voltage
    measurement = battery_voltage           #Voltage as Measurement
    rolling_average.add_measurement(measurement)    #Averaging the last 5 Voltages
    average = rolling_average.get_average()
    BatteryLevel = str(BatteryLevelinPer(average))       #Calculate Voltage to Percent

#########RPM from CAN Bus######################################################################
# Create a CAN bus channel
can_bus = can.interface.Bus(interface='socketcan', channel='can0', bitrate=1000000)

def getRPM(self):
    print("CAN Receiver: Waiting for messages...")
    while True:
        # Receive a message
        message = can_bus.recv(0.1)  # Adjust timeout as needed
        if message:
            print(f"Received message: {message}")
            # Process the message or return it as needed
            Rpm = str(message.data[0])
            return str(message.data[0])
        else:
            # Add a small delay to avoid excessive CPU usage
            GLib.MainContext.default().iteration(False)

######### DBus Sender ######################################################################
class MyDBUSService:
    """
    <node>
        <interface name='net.lew21.pydbus.ClientServerExample11'>
            <method name='RPM'>
                <arg type='s' name='response' direction='out'/>
            </method>
            <method name='BatteryLevel'>
                <arg type='s' name='response' direction='out'/>
            </method>
            <method name='Quit'/>
        </interface>
    </node>
    """
    def __init__(self, rpm, batterylevel):
        self.rpm = rpm
        self.batterylevel = batterylevel

    def RPM(self):
        return self.rpm
    
    def BatteryLevel(self):
        return self.batterylevel

    def Quit(self):
        """removes this object from the DBUS connection and exits"""
        loop.quit()

def loop_main():
    with concurrent.futures.ThreadPoolExecutor() as executor:
        f1 = executor.submit(getRPM)
        f2 = executor.submit(getBatteryLevel)
    MyDBUSService(Rpm, BatteryLevel).RPM
    MyDBUSService(Rpm, BatteryLevel).BatteryLevel

bus = SessionBus()
bus.publish("net.lew21.pydbus.ClientServerExample11", MyDBUSService())

loop = GLib.MainLoop()

# Start the RPM method in a non-blocking way
GLib.idle_add(loop_main)
loop.run()
