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
		self.assertLineCount('testcorrelator.log', expr='Invalid random source specified \\(must be SOURCE_MERSEINNE, SOURCE_LCG or SOURCE_DEVICE\\)', condition='==1')
		self.assertLineCount('testcorrelator.log', expr='Invalid distribution specified', condition='==1')
		self.assertLineCount('testcorrelator.log', expr='Invalid output format specified \\(must be OUTPUT_REAL, OUTPUT_INTEGER or OUTPUT_BOOL\\)', condition='==1')
		self.assertLineCount('testcorrelator.log', expr='Invalid combination of distribution and output format specified', condition='==16')
		self.assertLineCount('testcorrelator.log', expr='Invalid distribution arguments specified', condition='==34')
