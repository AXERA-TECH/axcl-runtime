# child normal: Launch main_app with child_app (type 0:normal, 8 seconds)
./main_app -t -1 -c "./child_app -t 0 -s 8"


# child crush: Launch main_app with child_app (type 1: crush, 8 seconds)
./main_app -t -1 -c "./child_app -t 1 -s 8"


# parent crush（after 10s）: Launch main_app with child_app (type 0, no sleep specified)
./main_app -t -1 -c "./child_app -t 0"
