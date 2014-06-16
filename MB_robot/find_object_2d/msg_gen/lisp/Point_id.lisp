; Auto-generated. Do not edit!


(cl:in-package find_object_2d-msg)


;//! \htmlinclude Point_id.msg.html

(cl:defclass <Point_id> (roslisp-msg-protocol:ros-message)
  ((id
    :reader id
    :initarg :id
    :type cl:fixnum
    :initform 0)
   (punto
    :reader punto
    :initarg :punto
    :type geometry_msgs-msg:Point
    :initform (cl:make-instance 'geometry_msgs-msg:Point)))
)

(cl:defclass Point_id (<Point_id>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Point_id>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Point_id)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name find_object_2d-msg:<Point_id> is deprecated: use find_object_2d-msg:Point_id instead.")))

(cl:ensure-generic-function 'id-val :lambda-list '(m))
(cl:defmethod id-val ((m <Point_id>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader find_object_2d-msg:id-val is deprecated.  Use find_object_2d-msg:id instead.")
  (id m))

(cl:ensure-generic-function 'punto-val :lambda-list '(m))
(cl:defmethod punto-val ((m <Point_id>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader find_object_2d-msg:punto-val is deprecated.  Use find_object_2d-msg:punto instead.")
  (punto m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Point_id>) ostream)
  "Serializes a message object of type '<Point_id>"
  (cl:let* ((signed (cl:slot-value msg 'id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'punto) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Point_id>) istream)
  "Deserializes a message object of type '<Point_id>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'id) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'punto) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Point_id>)))
  "Returns string type for a message object of type '<Point_id>"
  "find_object_2d/Point_id")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Point_id)))
  "Returns string type for a message object of type 'Point_id"
  "find_object_2d/Point_id")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Point_id>)))
  "Returns md5sum for a message object of type '<Point_id>"
  "c12f20f6b5bab8a20acb724d501bc937")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Point_id)))
  "Returns md5sum for a message object of type 'Point_id"
  "c12f20f6b5bab8a20acb724d501bc937")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Point_id>)))
  "Returns full string definition for message of type '<Point_id>"
  (cl:format cl:nil "int16 id~%geometry_msgs/Point punto~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Point_id)))
  "Returns full string definition for message of type 'Point_id"
  (cl:format cl:nil "int16 id~%geometry_msgs/Point punto~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Point_id>))
  (cl:+ 0
     2
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'punto))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Point_id>))
  "Converts a ROS message object to a list"
  (cl:list 'Point_id
    (cl:cons ':id (id msg))
    (cl:cons ':punto (punto msg))
))
