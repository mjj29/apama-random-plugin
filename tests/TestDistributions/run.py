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
			"Testing uniformInt",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"Testing uniformFloat",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing bernoulli",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"any\\(boolean,",
			"Testing binomial",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"Testing negbinomial",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"Testing geometric",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"Testing poisson",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"any\\(integer,",
			"Testing exponential",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing gamma",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing weibull",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing extreme",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing normal",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing lognormal",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing chisquared",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing cauchy",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing fisher",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"Testing student",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			"any\\(float,",
			])
