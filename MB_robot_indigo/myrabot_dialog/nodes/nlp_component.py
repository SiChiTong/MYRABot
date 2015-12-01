#!/usr/bin/env python
import rospy
from std_msgs.msg import String

import os

def f_publisher(str):
    #rospy.init_node('talker', anonymous=True)
    rospy.loginfo(str)
    pub.publish(str)
    

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + " I heard %s",data.data)
    mystring = data.data.split(' ')
    print "Word #: %s" % ( mystring )
    print "Word #: %d" % ( len(mystring) )
    
    newstr = data.data
    
    #Check for the place
    if data.data.upper().startswith('MOVE') or data.data.upper().startswith('GO'): 
      print "MOVEMENT ACTION "        
      
      #Check for the location
      for i in range(len(r_location)):
        #print i, self.question[i]
        if r_location[i] in data.data.upper():
          print "LOCATION: %s" % ( r_location[i] )
    
      #Check for the place
      for i in range(len(r_locationplace)):
        #print i, self.question[i]
        if r_locationplace[i] in data.data.upper():
          print "PLACE: %s" % ( r_locationplace[i] )
          
      #Check for the fetch something
      #for i in range(len(r_locationplace)):
        #print i, self.question[i]
      if 'GRASP' in data.data.upper():
        print "EXTRA ACTION: %s" % ( 'GRASPING' )

        #Check for the item
        for i in range(len(r_item)):
          #print i, self.question[i]
          if r_item[i] in data.data.upper():
            print "ITEM: %s" % ( r_item[i] )
            
        for i in range(len(r_personal)):
          #print i, self.question[i]
          if r_personal[i] in data.data.upper():
            print "GIVE TO: %s" % ( r_personal[i] )
            
    elif data.data.upper().startswith('SPECIAL'):
      newstr = data.data.upper().replace("SPECIAL COMMAND PLEASE ROBOT", "")
      print "SPECIAL ACTION:%s " % (newstr) 
    elif data.data.upper().startswith('PLEASE'):
      newstr = data.data.upper().replace("PLEASE ROBOT", "")
      print "HELP ACTION: "
    else:
      print "GAME MODE"

    f_publisher(data)
    
def listener():
    global r_location;
    global r_item;
    global r_locationplace;
    global r_personal;
    global r_mode;
    
  
    r_location      = []
    r_item          = []
    r_locationplace = []
    r_personal      = []
    r_mode          = []
    #os.system("pwd")
    
    # get rid of empty lines and do not use anything that starts with a '#'
    for loc in [location.strip('\n') for location in open('/home/fran/catkin_ws/src/messenger_2/config/locations.txt', 'r').readlines()]:
        if loc != '':
            if loc[0] != '#':
                r_location.append(loc)
    
    for it in [location_category.strip('\n') for location_category in open('/home/fran/catkin_ws/src/messenger_2/config/items.txt', 'r').readlines()]:
        if it != '':
            if it[0] != '#':
                r_item.append(it)
                
    # get rid of empty lines and do not use anything that starts with a '#'
    for loc_pla in [location.strip('\n') for location in open('/home/fran/catkin_ws/src/messenger_2/config/location_place.txt', 'r').readlines()]:
        if loc_pla != '':
            if loc_pla[0] != '#':
                r_locationplace.append(loc_pla)
    
    for pnoun in [location_category.strip('\n') for location_category in open('/home/fran/catkin_ws/src/messenger_2/config/personalnoun.txt', 'r').readlines()]:
        if pnoun != '':
            if pnoun[0] != '#':
                r_personal.append(pnoun)

                # get rid of empty lines and do not use anything that starts with a '#'
    for mode in [location.strip('\n') for location in open('/home/fran/catkin_ws/src/messenger_2/config/mode.txt', 'r').readlines()]:
        if mode != '':
            if mode[0] != '#':
                r_mode.append(mode)
    

    # in ROS, nodes are unique named. If two nodes with the same
    # node are launched, the previous one is kicked off. The 
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaenously.
    rospy.init_node('nlp_component', anonymous=True)

    rospy.Subscriber("/asr_component/asr_output", String, callback)
    global pub
    pub = rospy.Publisher('~nlp_output', String)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
        
if __name__ == '__main__':
    listener()
