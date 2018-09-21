from pysys.constants import *
from apama.basetest import ApamaBaseTest
from apama.correlator import CorrelatorHelper


class PySysTest(ApamaBaseTest):
	def execute(self):
		correlator = CorrelatorHelper(self, name='testcorrelator')
		correlator.start(logfile='testcorrelator.log')
		correlator.injectEPL(filenames=['RandomPlugin.mon'], filedir=PROJECT.APAMA_WORK+'/monitors')
		correlator.injectEPL(filenames=['test.mon'])
		correlator.flush() 

	def validate(self):
		self.assertGrep('testcorrelator.log', expr='ERROR', contains=False)
		self.assertOrderedGrep('testcorrelator.out', exprList=[
				"Testing mersienne",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"Testing mersienne seeded",
				"any\\(integer,51\\)",
				"any\\(integer,28\\)",
				"any\\(integer,51\\)",
				"any\\(integer,-73\\)",
				"any\\(integer,81\\)",
				"any\\(integer,-82\\)",
				"any\\(integer,15\\)",
				"any\\(integer,-26\\)",
				"any\\(integer,-45\\)",
				"any\\(integer,-22\\)",
				"any\\(integer,-98\\)",
				"Testing lcg",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"Testing lcg seeded",
				"any\\(integer,-100\\)",
				"any\\(integer,5\\)",
				"any\\(integer,47\\)",
				"any\\(integer,-48\\)",
				"any\\(integer,-25\\)",
				"any\\(integer,-61\\)",
				"any\\(integer,96\\)",
				"any\\(integer,2\\)",
				"any\\(integer,6\\)",
				"any\\(integer,-49\\)",
				"any\\(integer,-79\\)",
				"Testing hardware",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
				"any\\(integer",
			])
