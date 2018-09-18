# apama-random-plugin
Apama EPL plugin for generating random numbers for statistical applications

## Building the plugin

To build the plugin run the following command from an Apama command prompt on Linux:

    g++ -std=c++17 -o $APAMA\_WORK/lib/librandom.so -I$APAMA\_HOME/include -L$APAMA_HOME/lib -lapclient -shared -fPIC random.cpp

On Windows run this command:

    g++ -std=c++17 -o %APAMA\_WORK%\\lib\\librandom.so -I%APAMA\_HOME%\\include -L%APAMA_HOME%\\lib -lapclient -shared -fPIC random.cpp

To generate the Apama documentation for the RandomPlugin module run this command on Linux:

   java -jar $APAMA\_HOME/lib/ap-generate-apamadoc.jar plugin/doc plugin/eventdefinitions

Or on Windows:

   java -jar %APAMA\_HOME%\\lib\\ap-generate-apamadoc.jar plugin\\doc plugin\\eventdefinitions

## Using the plugin

To use the plugin first inject RandomPlugin.mon, then use the static methods on the `RandomPlugin` class to create a `Generator` instance which you can use to get values from the sequence:

    com.apamax.random.Generator g := com.apamax.random.RandomPlugin.createUniformIntGenerator(3, 17);
	 integer r1 := <integer> g.generate();
	 integer r2 := <integer> g.generate();

This will get you a randomly seeded generator. If you want reproducible sequences from a specified seed, call `setSeed()` on the generator before generating any values.

There are various convenience functions usch as the `createUniformIntGenerator` used above for creating common sequences. Full access to all of the options for both sources of random data and distributions can be had using the `createGenerator` method and the various constants on `RandomPlugin`. For example:

    using com.apamax.random.Generator;

	 ...

    Generator g := RandomPlugin.createGenerator(RandomPlugin.SOURCE_MERSIENNE, RandomPlugin.OUTPUT_FLOAT, RandomPlugin.DIST_NORMAL, [<any> 10.0, <any> 1.0]); // mean and stdev of sequence
	 float r1 := <float> g.generate();
	 float r2 := <float> g.generate();

For more details on the available convenience functions, sources, distributions and outputs consult the API documentation for RandomPlugin.

