pid=$(ps -ef | grep 'floor_20' | grep -v grep | awk '{print $2}')
python3 trigger.py $pid -1

pid=$(ps -ef | grep 'floor_14' | grep -v grep | awk '{print $2}')
python3 trigger.py $pid 1

pid=$(ps -ef | grep 'floor_4' | grep -v grep | awk '{print $2}')
python3 trigger.py $pid 1

pid=$(ps -ef | grep 'floor_10' | grep -v grep | awk '{print $2}')
python3 trigger.py $pid -1