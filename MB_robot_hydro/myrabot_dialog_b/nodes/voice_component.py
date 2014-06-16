#!/usr/bin/env python

#***********************************************************
#* Software License Agreement (BSD License)
#*
#*  Copyright (c) 2009, Willow Garage, Inc.
#*  All rights reserved.
#*
#*  Redistribution and use in source and binary forms, with or without
#*  modification, are permitted provided that the following conditions
#*  are met:
#*
#*   * Redistributions of source code must retain the above copyright
#*     notice, this list of conditions and the following disclaimer.
#*   * Redistributions in binary form must reproduce the above
#*     copyright notice, this list of conditions and the following
#*     disclaimer in the documentation and/or other materials provided
#*     with the distribution.
#*
#*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#*  POSSIBILITY OF SUCH DAMAGE.
#***********************************************************


# Import after printing usage for speed.
import rospy
import roslib; 
from sound_play.msg import SoundRequest
from sound_play.libsoundplay import SoundClient
import sys
from std_msgs.msg import String

from myrabot_dialog_b.srv import *
   

class VoiceGenerator:
    def __init__(self):
        rospy.on_shutdown(self.cleanup)

        #self.voice = rospy.get_param("~voice", "voice_kal_diphone")
        self.voice = rospy.get_param("~voice", "voice_don_diphone")
        self.wavepath = rospy.get_param("~wavepath", "")
        self.topic = rospy.get_param("~voice_topic", "")
       
        # Create the sound client object
        self.soundhandle = SoundClient()
        
        rospy.sleep(1)
        self.soundhandle.stopAll()
       
        # Announce that we are ready for input
        rospy.sleep(1)
        self.soundhandle.say("Voice Module Ready", self.voice)

        rospy.loginfo("Message ...")

        # Subscribe to the recognizer output
        rospy.Subscriber(self.topic, String, self.voicegenerator)
    
    def switch_asr_onoff_client(self, x):
        rospy.wait_for_service('switch_asr_on_off')
        try:
            switch_asr_on_off = rospy.ServiceProxy('switch_asr_on_off', asr_status)
            resp1 = switch_asr_on_off(x)
            
        except rospy.ServiceException, e:
            print "Service call failed: %s"%e
            
        
    def voicegenerator(self, msg):
        # Print the recognized words on the screen
        rospy.loginfo(msg.data)
        
        #if we have problems between voice generator and asr uncomment this
        #response = self.switch_asr_onoff_client(0)
        #rospy.sleep(1)
        
        # Speak the recognized words in the selected voice
        self.soundhandle.say(msg.data, self.voice)

        #if we have problems between voice generator and asr uncomment this
        #response = self.switch_asr_onoff_client(1)


        # Uncomment to play one of the built-in sounds
        #rospy.sleep(2)
        #self.soundhandle.play(5)
       
        # Uncomment to play a wave file
        #rospy.sleep(2)
        #self.soundhandle.playWave(self.wavepath + "/R2D2a.wav")

    def cleanup(self):
        rospy.loginfo("Shutting down voicegenerator node...")

if __name__=="__main__":
    rospy.init_node('voicegenerator', anonymous = True)
    try:
        VoiceGenerator()
        rospy.spin()
    except:
        pass