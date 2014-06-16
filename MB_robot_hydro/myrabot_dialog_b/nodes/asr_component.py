#!/usr/bin/env python

"""
recognizer.py is a wrapper for pocketsphinx.
  parameters:
    ~lm   - filename of language model
    ~dict - filename of dictionary
    ~fsg  - Finite State Grammar
  publications:
    ~output (std_msgs/String) - text output
  services:
    ~start (std_srvs/Empty) - start speech recognition
    ~stop (std_srvs/Empty) - stop speech recognition
  dependencies:   
    ~sound play: If you want to talk 
        #rosrun sound_play soundplay_node.py
"""
# rospy should provide you with all the APIs you need to run a node, call 
# services, access the Parameter Server, etc... roslib is primarily an 
# internal library used by ROS core developers to write tools. 
import roslib; roslib.load_manifest('myrabot_dialog_b')
import rospy

import os
import time


import pygtk
pygtk.require('2.0')
import gtk

import gobject
import pygst
pygst.require('0.10')
gobject.threads_init()
import gst

from std_msgs.msg import String
from std_srvs.srv import *

from myrabot_dialog_b.srv import *


class ASR_Recognizer(object):
    """ GStreamer based speech recognizer. """
    
    def __init__(self):
        
        """ Initialize the speech pipeline components. """
        rospy.init_node('asr_component')
        self.pub = rospy.Publisher('~asr_output',String)
        service_on = rospy.Service('switch_asr_on_off',  asr_status, self.switch_onoff)
        rospy.on_shutdown(self.shutdown)

        self.status     = 0
        self.laststring = ' ' 
        self.noise      = 0
        self.bored      = 0
        self.mode       = 4 #FREE MODE
        self.MESSAGE_ON = 1 #DEBUG mode with 1
        self.DEBUG_ON   = 0
        self
        # services to start/stop recognition
        rospy.Service("~start", Empty, self.start)
        rospy.Service("~stop", Empty, self.stop)

        # configure pipeline
        self.pipeline = gst.parse_launch('gconfaudiosrc ! audioconvert ! audioresample '
                                         + '! vader name=vad auto-threshold=true '
                                         + '! pocketsphinx name=asr ! fakesink')
        asr = self.pipeline.get_by_name('asr')
        asr.connect('partial_result', self.asr_partial_result)
        asr.connect('result', self.asr_result)
        asr.set_property('configured', True)
        asr.set_property('dsratio', 1)

        # parameters for lm and dic
        try:
            lm_ = rospy.get_param('~lm')
            asr.set_property('lm', lm_)
        except:
            try:
                fsg_ = rospy.get_param('~fsg')
                asr.set_property('fsg', fsg_)
            except:
                rospy.logerr('Please specify a language model file or a fsg grammar file')
                return
        try:
            dict_ = rospy.get_param('~dict')
        except:
            rospy.logerr('Please specify a dictionary')
            return

        asr.set_property('dict', dict_)
        bus = self.pipeline.get_bus()
        bus.add_signal_watch()
        bus.connect('message::application', self.application_message)
        self.start(None)
        self.start_greetings(None)

        gtk.main()
        
        
    def switch_onoff(self, req):
        """ Shutdown the GTK thread. """
        print ">>>>>>>>>>>>>>>>>>>>>>>>> switch_onoff <<<<<<<<<<<<<<<<<<<<<<<<<<<<"
        print req
        
        if req.a == 1:
          self.start(None)
        else:
          self.stop(None)
          
        return 0

        
    def shutdown(self):
        """ Shutdown the GTK thread. """
        gtk.main_quit()

    def start(self, req):
        self.pipeline.set_state(gst.STATE_PLAYING)
        print ">>>>>>>>>>>>>>>>>>>>>>>>> STARTING ASR <<<<<<<<<<<<<<<<<<<<<<<<<<<<"
        #self.start_greetings()
        return EmptyResponse()

    def start_greetings(self, req):
        self.voice_message("Welcome to Myrabot Dialog System" )
        return;
        
    def voice_message(self, chain):
        print ">>>>>>>>>>>>>>>>>>>>>>>>> Voice test without dialog package <<<<<<<<<<<<<<<<<<<<<<<<<<<<"
        if self.DEBUG_ON:
          msgtest = "rosrun sound_play say.py \""
          msgtest += chain
          msgtest += "\""
          os.system(msgtest)
        
    def stop(self, req):
        self.pipeline.set_state(gst.STATE_PAUSED)
        print "<<<<<<<<<<<<<<<<<<<<<<<<<<<< STOPPING ASR >>>>>>>>>>>>>>>>>>>>>>>>>"
        #vader = self.pipeline.get_by_name('vad')
        #vader.set_property('silent', True)
        return EmptyResponse()

    def asr_partial_result(self, asr, text, uttid):
        """ Forward partial result signals on the bus to the main thread. """
        struct = gst.Structure('partial_result')
        struct.set_value('hyp', text)
        struct.set_value('uttid', uttid)
        asr.post_message(gst.message_new_application(asr, struct))

    def asr_result(self, asr, text, uttid):
        """ Forward result signals on the bus to the main thread. """
        struct = gst.Structure('result')
        struct.set_value('hyp', text)
        struct.set_value('uttid', uttid)
        asr.post_message(gst.message_new_application(asr, struct))

    def application_message(self, bus, msg):
        """ Receive application messages from the bus. """
        msgtype = msg.structure.get_name()
        if msgtype == 'partial_result':
            self.partial_result(msg.structure['hyp'], msg.structure['uttid'])
        if msgtype == 'result':
            self.final_result(msg.structure['hyp'], msg.structure['uttid'])

    def partial_result(self, hyp, uttid):
        """ Delete any previous selection, insert text and select it. """
        if self.MESSAGE_ON == 1:
          print "Partial: " + hyp
        
        mystring = hyp.split(' ')
        
        if self.MESSAGE_ON:
          print "Word #: %s" % ( mystring )
          print "Word #: %d" % ( len(mystring) )
          
        if((self.status == 1) and (len(mystring) > 1)):
            self.noise = 1
        elif ((self.status == 1) and (len (mystring) == 1)):
            self.noise = 0
        
        ## function for parsing the data
        #def data_parser(text, dic):
            #for i, j in dic.iteritems():
                #text = text.replace(i,j)
        #return text
        
    def final_result(self, hyp, uttid):
        """ Insert the final result. """
        msg = String()
        msg.data = str(hyp.lower())
        #rospy.loginfo(msg.data)
        #self.pub.publish(msg)
        
        mystring = msg.data.split(' ')
        print "Word #: %s" % ( mystring )
        print "Word #: %d" % ( len(mystring) )

        newstr = msg.data

        if "MODE" in newstr.upper():
          self.stop(None)
          
          if "BASIC MODE" in newstr.upper():
            print "MODE #: %s" % ( newstr.upper() )
            self.voice_message('Changing to basic mode')
            self.mode = 0
          elif "GRASPING MODE" in newstr.upper():
            print "MODE #: %s" % ( newstr.upper() )
            self.voice_message('Changing to grasping mode')
            self.mode = 1
          elif "SEMANTIC NAVIGATION MODE" in newstr.upper():
            print "MODE #: %s" % ( newstr.upper() )
            self.voice_message('Changing to semantic navigation mode')
            self.mode = 2
          elif "MIRROR MODE" in newstr.upper():
            print "MODE #: %s" % ( newstr.upper() )
            self.voice_message('Changing to mirror mode')
            self.mode = 3
          elif "FREE MODE" in newstr.upper():
            print "MODE #: %s" % ( newstr.upper() )
            self.voice_message('Changing to Free mode')
            self.mode = 4 #FreeMode is the OFF mode
          
          time.sleep( 2 )
          self.start(None)
          
          rospy.loginfo("MODE " + str(self.mode))
          self.pub.publish("MODE " +  str(self.mode))
        else :  

          if self.mode == 3:
              self.stop(None)
              self.voice_message( newstr )
              self.bored += 1
              if ( self.bored % 5 ) == 0:
                self.voice_message( "I am too tired of this game, plase change my mode or I will die" )
              time.sleep( 2 )
              self.start(None)
          elif self.mode == 4:
              print "FREE MODE #: %s" % ( 'ASR is not working' )
          else:
            newstr = msg.data.replace("please", "")
            
            if self.MESSAGE_ON:
                rospy.loginfo("MODE " + str(self.mode))
                  
            if ((mystring[0] != 'yes' and mystring[0] != 'no' ) and 
                (self.status == 0)    and (len(mystring) > 1) ):
                self.stop(None)
                
                self.voice_message( "Did you say " + newstr )
                self.laststring = newstr.replace("myrabot", "")
                print "WORD: laststring #: %s" % ( self.laststring )

                self.status = 1
                time.sleep( 2 )
                self.start(None)
            elif ((mystring[0] == 'yes') and (self.status == 1) and 
                (len(mystring) == 1)     and (self.noise == 0)):
                self.stop(None)
                print "WORD: laststring #: %s" % ( self.laststring )

                if self.mode == 0:
                  if "move" in self.laststring or "go"  in self.laststring :
                    self.voice_message( "OK, I am going to  " + self.laststring )
                  else :
                    self.voice_message( "This is not a movement command" )

                elif self.mode == 1:
                  if "grasp" in self.laststring:
                    self.voice_message( "OK, I am going to " + self.laststring )
                  else :
                    self.voice_message( "This is not a grasping command" )

                elif self.mode == 2:
                  if "save in your map" in self.laststring:
                    newstr = self.laststring.replace("this is the", "")
                    newstr = newstr.replace("save in your map", "")
                    self.voice_message( "OK, You just associate this pose with the " + newstr )
                  else :
                    self.voice_message( "This is not a semantic command" )
  
                time.sleep( 2 )
                rospy.loginfo(self.laststring)
                self.pub.publish(self.laststring)
                self.laststring = ' '
                self.status = 0
                self.start(None)
            elif (( mystring[0]   == 'no') and (self.status==1) and 
                  ( len(mystring) == 1)    and (self.noise == 0)):
                self.stop(None)
                self.voice_message( "Could you repeat the question please?" )
                self.status = 0
                time.sleep( 1 )
                self.start(None)


        
if __name__=="__main__":
    i_asr_recognizer = ASR_Recognizer()

    
