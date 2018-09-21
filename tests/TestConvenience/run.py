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
		self.assertGrep('testcorrelator.out', expr='Testing uniformInt')
		self.assertGrep('testcorrelator.out', expr='Testing uniformFloat')
		self.assertGrep('testcorrelator.out', expr='Testing binomial')
		self.assertGrep('testcorrelator.out', expr='Testing normal')
		self.assertGrep('testcorrelator.out', expr='Testing poisson')
		self.assertLineCount('testcorrelator.out', expr='any\\(integer', condition='==33')
		self.assertLineCount('testcorrelator.out', expr='any\\(float', condition='==22')
