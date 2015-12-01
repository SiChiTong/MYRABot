#!/usr/bin/env python
import rospy
from std_msgs.msg import String

import os

def talker(str):
    #rospy.init_node('talker', anonymous=True)
    rospy.loginfo(str)
    pub_voice.publish(str)
    

def nlp_callback(data):
    rospy.loginfo(rospy.get_caller_id() + " I heard %s",data.data)
    mystring = data.data.split(' ')
    print "Word #: %s" % ( mystring )
    print "Word #: %d" % ( len(mystring) )
    
    newstr = data.data
    
    talker(data)
    
def listener():
 

    # in ROS, nodes are unique named. If two nodes with the same
    # node are launched, the previous one is kicked off. The 
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaenously.
    rospy.init_node('dialog_component', anonymous=True)
    
    #subscription to nlp_component
    rospy.Subscriber("/nlp_component/nlp_output", String, nlp_callback)
    
    global pub_upper
    global pub_voice
    pub_voice = rospy.Publisher('/voice_component/voice_component_input', String)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
        
if __name__ == '__main__':
    listener()
