import rospy
from std_msgs.msg import Bool
import serial
import time


class arduinoread:
  def __init__(self):
    rospy.init_node('arduino_talker', anonymous=True)
    self.arduino_top_lim=rospy.get_param('~temp_topic')
    self.arduino_bot_lim=rospy.get_param('~temp_topic')
        
    self.comport=rospy.get_param('~com_port')
        
    self.ser = None
        
    self.top_lim_pub = rospy.Publisher(self.arduino_top_lim, Bool, queue_size=1)
    self.bot_lim_pub = rospy.Publisher(self.arduino_bot_lim, Bool, queue_size=1)
    time.sleep(1)
    rospy.Timer(rospy.Duration(0.04),self.read_serial)


  def read_serial(self,event):
    if not rospy.is_shutdown():
      try:
        if(self.ser == None):
          rospy.loginfo("Trying to reconnect to serial")
          self.ser = serial.Serial(self.comport,115200,timeout=0.04)
          rospy.loginfo("Connected to serial")

        self.handle_serial_data(self.ser.read())
        self.ser.reset_input_buffer()
      except serial.serialutil.SerialException:
        if(not(self.ser == None)):
          self.ser.close()
          self.ser = None
          rospy.logwarn("Disconnecting from serial")
        rospy.logwarn("Serial disconnected")
    else:
      rospy.signal_shutdown("system shutdown")

  def handle_serial_data(self, raw_serial_data):
    try:
      output = int(raw_serial_data.decode('utf-8'))
    except:
      rospy.logwarn("Garbage serial data")
      return
    mes_top_lim = Bool()
    mes_bot_lim = Bool()

    if(output == 0):
      mes_top_lim.data = 0
      mes_bot_lim.data = 0
    if(output == 1):
      mes_top_lim.data = 0
      mes_bot_lim.data = 1
    if(output == 2):
      mes_top_lim.data = 1
      mes_bot_lim.data = 0
    if(output == 3):
      mes_top_lim.data = 1
      mes_bot_lim.data = 1


    self.top_lim_pub.publish(mes_top_lim)
    self.bot_lim_pub.publish(mes_bot_lim)


if __name__ == '__main__':
  arduinoreader=arduinoread()
  rospy.spin()