import roslib; roslib.load_manifest('json_prolog')
 
import rospy
import json_prolog
from json_prolog import json_prolog


def listenCallback(data):
	request = data.data
        print "get voice"
	rospy.loginfo(rospy.get_caller_id() + 'I heard %s', request)
	prolog = json_prolog.Prolog()
    	query = prolog.query("owl_has(herb:'data.data',P,O)")

	for sol in query.solutions():
        	print sol
    	query.finish()
	rospy.spin()


def listener():
	rospy.init_node('medicineTest', anonymous=False)
	rospy.Subscriber('xfspeech', String, listenCallback)
    	prolog = json_prolog.Prolog()
    	query = prolog.query("owl_has(herb:'名称',P,O)")
   	
	for sol in query.solutions():
        	print sol
    	query.finish()
	rospy.spin()


if __name__ == '__main__':
	print "listening"
	listener()
 





#!/usr/bin/env python
# -*- coding:utf-8 -*-


import roslib; roslib.load_manifest('json_prolog')
 
import rospy
from json_prolog import json_prolog
from std_msgs.msg import String


if __name__ == '__main__':
    rospy.init_node('test_json_prolog')
    prolog = json_prolog.Prolog()
    query = prolog.query("owl_has(herb:'毒性', P, O)")
    for solution in query.solutions():
        print 'Found solution. A = %s, B = %s' % (solution['P'], solution['O'])
    query.finish()

