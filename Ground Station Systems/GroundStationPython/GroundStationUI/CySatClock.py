import threading
import logging
import datetime
import time

#   Class to display the time every seconds
#   Every 5 seconds, the time is displayed using the logging.ERROR level
#   to show that different colors are associated to the log levels
class Clock(threading.Thread):

    def __init__(self, logger):
        super().__init__()
        self._stop_event = threading.Event()
        self._logger = logger

    def run(self):
        self._logger.debug('Clock started')
        previous = -1
        while not self._stop_event.is_set():
            now = datetime.datetime.now()
            if previous != now.second:
                previous = now.second
                if now.second % 5 == 0:
                    level = logging.ERROR
                else:
                    level = logging.INFO
                # Remove comment below to get full logging
                # logger.log(level, now)
            time.sleep(0.2)

    def stop(self):
        self._stop_event.set()