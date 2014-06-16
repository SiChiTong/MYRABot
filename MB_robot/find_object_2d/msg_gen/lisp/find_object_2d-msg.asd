
(cl:in-package :asdf)

(defsystem "find_object_2d-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "Point_id" :depends-on ("_package_Point_id"))
    (:file "_package_Point_id" :depends-on ("_package"))
    (:file "PointObjects" :depends-on ("_package_PointObjects"))
    (:file "_package_PointObjects" :depends-on ("_package"))
  ))