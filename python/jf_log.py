
from loguru import logger
from functools import partialmethod
import sys


class JfLogger:
    def __init__(self, log_path="cur.log") -> None:
        self.logger = logger
        self.logger.remove()
        self.logger.level("INFO", color="<green>")
        self.logger.level("DEBU", color="<blue>", no=15)
        self.logger.level("EROR", color="<red>", no=44)
        self.logger.level("TRAC", color="<white><GREEN>", no=110)
        self.logger.__class__.t = partialmethod(self.logger.__class__.log, "TRAC")
        self.logger.__class__.e = partialmethod(self.logger.__class__.log, "EROR")
        self.logger.__class__.i = partialmethod(self.logger.__class__.log, "INFO")
        self.logger.__class__.d = partialmethod(self.logger.__class__.log, "DEBU")
        self.logger.add(sys.stdout,colorize=True,
                        format="<green>[</green><level>{level}</level><green>]</green>"
                        "<green>[</green><cyan>{line}</cyan>:<cyan>{function}</cyan><green>]</green>"
                        "<level>{message}</level>",
                        level=10
        );
    def get_logger(self):
        return self.logger
    
jf_log=JfLogger().get_logger()