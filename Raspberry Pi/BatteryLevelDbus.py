from pydbus import SessionBus
from gi.repository import GLib                                          #Lib for MainLoop handling
from piracer.vehicles import PiRacerStandard                            #Lib for Piracer Data

class RollingAverage:                                                   #Averaging the last 5 Measurements
    def __init__(self, window_size=5):                                  #Averaging Window for 5 Values
        self.window_size = window_size
        self.measurements = []

    def add_measurement(self, value):                                   #Add Values to measurementsList 
        self.measurements.append(value)
        if len(self.measurements) > self.window_size:                   #If more Values than WindowsSize 5 then remove the first(oldest)
            self.measurements = self.measurements[-self.window_size:]

    def get_average(self):                                              #Return the Average of measurementsList
        if not self.measurements:
            return None                                                 #Return None if no measurements are available
        return sum(self.measurements) / len(self.measurements)

class BatteryService(object):                                           #Dbus Object for the BatteryService, DocString is important for Dbus
  """
      <node>
          <interface name='com.dbus.batteryService'>
              <method name='getBatteryLevel'>
                  <arg type='s' name='response' direction='out'/>
              </method>
              <method name='getConsumption'>
                  <arg type='s' name='response' direction='out'/>
              </method>
              <method name='getVoltage'>
                  <arg type='s' name='response' direction='out'/>
              </method>
              <method name='getCurrent'>
                <arg type='s' name='response' direction='out'/>
              </method>
          </interface>
      </node>
  """
  def __init__(self, vehicle):                                          #Initialize Values (vehicle is not used anymore)
    self._current = 0.0
    self._voltage = 0.0
    self._consumption = 0.0
    self._batterylevel = 0.0
    self._vehicle = vehicle

  def getBatteryLevel(self)-> str:                                      #Get Batterylevel: Get current Voltage and calculate BatteryLevel in 100%
    rolling_average = RollingAverage()                                  #Create RollingAverage object
    self._voltage  = self._vehicle.get_battery_voltage()                #Get Voltage from Piracer
    rolling_average.add_measurement(self._voltage )                     #Adding the last 5 Voltages to MeasurementsList
    x = rolling_average.get_average()                                   #Get the rolling Average
    #Calculate Voltage to Percent: Function was interpolated based on Paper for this Battery
    self._batterylevel = -25.6269 * x**3 + 887.9706 * x**2 - 10180.5203 * x + 38661.8396     
    return str(round(self._batterylevel, 3))                            #Return Batterylevel in % as String with 3 Digits after ,

  def getVoltage(self) -> str:                                          #Get Voltage [V] from Piracer 
    _voltage          = round(abs(self._vehicle.get_battery_voltage()),3) 
    return str(_voltage)

  def getConsumption(self) -> str:                                      #Get Power in Watt [W] from Piracer
    _consumption      = round(abs(self._vehicle.get_power_consumption()), 3)
    return str(_consumption)

  def getCurrent(self) -> str:                                          #Get Current in Ampere [mA] from Piracer
    _current = round(abs(self._vehicle.get_battery_current()), 3)
    return str(_current)

def battery_service_start(vehicle: PiRacerStandard()):                  #Start the MainLoop to establish Dbus SessionBus (to Qt)
  loop = GLib.MainLoop()
  bus = SessionBus()
  bus.publish("com.dbus.batteryService", BatteryService(vehicle))       #Publish BatteryService to Dbus
  loop.run()

battery_service_start(PiRacerStandard())                                #Start the BatteryService