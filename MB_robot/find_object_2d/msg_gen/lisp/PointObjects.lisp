; Auto-generated. Do not edit!


(cl:in-package find_object_2d-msg)


;//! \htmlinclude PointObjects.msg.html

(cl:defclass <PointObjects> (roslisp-msg-protocol:ros-message)
  ((objeto
    :reader objeto
    :initarg :objeto
    :type (cl:vector find_object_2d-msg:Point_id)
   :initform (cl:make-array 0 :element-type 'find_object_2d-msg:Point_id :initial-element (cl:make-instance 'find_object_2d-msg:Point_id))))
)

(cl:defclass PointObjects (<PointObjects>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PointObjects>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PointObjects)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name find_object_2d-msg:<PointObjects> is deprecated: use find_object_2d-msg:PointObjects instead.")))

(cl:ensure-generic-function 'objeto-val :lambda-list '(m))
(cl:defmethod objeto-val ((m <PointObjects>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader find_object_2d-msg:objeto-val is deprecated.  Use find_object_2d-msg:objeto instead.")
  (objeto m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PointObjects>) ostream)
  "Serializes a message object of type '<PointObjects>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'objeto))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'objeto))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PointObjects>) istream)
  "Deserializes a message object of type '<PointObjects>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'objeto) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'objeto)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'find_object_2d-msg:Point_id))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PointObjects>)))
  "Returns string type for a message object of type '<PointObjects>"
  "find_object_2d/PointObjects")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PointObjects)))
  "Returns string type for a message object of type 'PointObjects"
  "find_object_2d/PointObjects")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PointObjects>)))
  "Returns md5sum for a message object of type '<PointObjects>"
  "f79b4adb458edaaf81faec054b53de74")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PointObjects)))
  "Returns md5sum for a message object of type 'PointObjects"
  "f79b4adb458edaaf81faec054b53de74")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PointObjects>)))
  "Returns full string definition for message of type '<PointObjects>"
  (cl:format cl:nil "Point_id[] objeto~%~%================================================================================~%MSG: find_object_2d/Point_id~%int16 id~%geometry_msgs/Point punto~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PointObjects)))
  "Returns full string definition for message of type 'PointObjects"
  (cl:format cl:nil "Point_id[] objeto~%~%================================================================================~%MSG: find_object_2d/Point_id~%int16 id~%geometry_msgs/Point punto~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PointObjects>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'objeto) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PointObjects>))
  "Converts a ROS message object to a list"
  (cl:list 'PointObjects
    (cl:cons ':objeto (objeto msg))
))
