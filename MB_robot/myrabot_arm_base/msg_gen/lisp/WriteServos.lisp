; Auto-generated. Do not edit!


(cl:in-package myrabot_arm_base-msg)


;//! \htmlinclude WriteServos.msg.html

(cl:defclass <WriteServos> (roslisp-msg-protocol:ros-message)
  ((posicion
    :reader posicion
    :initarg :posicion
    :type myrabot_arm_base-msg:Servos
    :initform (cl:make-instance 'myrabot_arm_base-msg:Servos))
   (velocidad
    :reader velocidad
    :initarg :velocidad
    :type myrabot_arm_base-msg:Servos
    :initform (cl:make-instance 'myrabot_arm_base-msg:Servos))
   (par
    :reader par
    :initarg :par
    :type myrabot_arm_base-msg:Servos
    :initform (cl:make-instance 'myrabot_arm_base-msg:Servos)))
)

(cl:defclass WriteServos (<WriteServos>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <WriteServos>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'WriteServos)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name myrabot_arm_base-msg:<WriteServos> is deprecated: use myrabot_arm_base-msg:WriteServos instead.")))

(cl:ensure-generic-function 'posicion-val :lambda-list '(m))
(cl:defmethod posicion-val ((m <WriteServos>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader myrabot_arm_base-msg:posicion-val is deprecated.  Use myrabot_arm_base-msg:posicion instead.")
  (posicion m))

(cl:ensure-generic-function 'velocidad-val :lambda-list '(m))
(cl:defmethod velocidad-val ((m <WriteServos>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader myrabot_arm_base-msg:velocidad-val is deprecated.  Use myrabot_arm_base-msg:velocidad instead.")
  (velocidad m))

(cl:ensure-generic-function 'par-val :lambda-list '(m))
(cl:defmethod par-val ((m <WriteServos>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader myrabot_arm_base-msg:par-val is deprecated.  Use myrabot_arm_base-msg:par instead.")
  (par m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <WriteServos>) ostream)
  "Serializes a message object of type '<WriteServos>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'posicion) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'velocidad) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'par) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <WriteServos>) istream)
  "Deserializes a message object of type '<WriteServos>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'posicion) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'velocidad) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'par) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<WriteServos>)))
  "Returns string type for a message object of type '<WriteServos>"
  "myrabot_arm_base/WriteServos")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'WriteServos)))
  "Returns string type for a message object of type 'WriteServos"
  "myrabot_arm_base/WriteServos")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<WriteServos>)))
  "Returns md5sum for a message object of type '<WriteServos>"
  "9551946156ffa6c4d13c8c60abd574c1")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'WriteServos)))
  "Returns md5sum for a message object of type 'WriteServos"
  "9551946156ffa6c4d13c8c60abd574c1")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<WriteServos>)))
  "Returns full string definition for message of type '<WriteServos>"
  (cl:format cl:nil "Servos posicion~%Servos velocidad~%Servos par~%~%================================================================================~%MSG: myrabot_arm_base/Servos~%int16 base~%int16 arti1~%int16 arti2~%int16 arti3~%int16 pinza~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'WriteServos)))
  "Returns full string definition for message of type 'WriteServos"
  (cl:format cl:nil "Servos posicion~%Servos velocidad~%Servos par~%~%================================================================================~%MSG: myrabot_arm_base/Servos~%int16 base~%int16 arti1~%int16 arti2~%int16 arti3~%int16 pinza~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <WriteServos>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'posicion))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'velocidad))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'par))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <WriteServos>))
  "Converts a ROS message object to a list"
  (cl:list 'WriteServos
    (cl:cons ':posicion (posicion msg))
    (cl:cons ':velocidad (velocidad msg))
    (cl:cons ':par (par msg))
))
