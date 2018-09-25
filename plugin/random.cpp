
#include <epl_plugin.hpp>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include <chrono>

using com::apama::epl::EPLPlugin;
using com::apama::epl::data_t;
using com::apama::epl::list_t;
using com::apama::epl::custom_t;
using com::apama::epl::get;

namespace com::apamax::random {

/** identifies the data_t type based on two bools, is the type integral or is it float */
template<bool I, bool F>
struct to_data_t_type;
template<>
struct to_data_t_type<true, false> { using type = int64_t; };
template<>
struct to_data_t_type<false, true> { using type = double; };

/** converts a type into the equivalent int or double type for data_t use */
template<typename T>
using to_data_t_type_t = typename to_data_t_type<std::is_integral<T>::value, std::is_floating_point<T>::value >::type;

/** returns true if T has a method seed */
template<typename T, typename=void>
struct has_seed_method: std::false_type {};

template<typename T>
struct has_seed_method<T, std::void_t<decltype(std::declval<T>().seed(std::declval<typename T::result_type>()))> >: std::true_type {};

/** creates a random engine seeded with a full state from std::random_device,
 * except on mingw where we just use the current hi-res time. */
template<class T = std::mt19937, std::size_t N = 624>
auto ProperlySeededRandomEngine () -> typename std::enable_if<!!N, T*>::type {
	if constexpr (has_seed_method<T>::value) {
#ifdef __MINGW32__
		// on mingw - random_device seems to always return the same thing (!)
		return new T(std::chrono::high_resolution_clock::now().time_since_epoch().count());
#else
		typename T::result_type random_data[N];
		std::random_device source;
		std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
		std::seed_seq seeds(std::begin(random_data), std::end(random_data));
		return new T(seeds);
#endif
	} else {
		return new T();
	}
}

class random_error: public std::runtime_error
{
	using std::runtime_error::runtime_error;
};


/** Random plugin class */
class Random: public EPLPlugin<Random>
{
	/** Base class for generator chunks */
	struct GeneratorBase
	{
		virtual ~GeneratorBase() {}
		virtual data_t next() = 0;
		virtual void seed(const data_t &seed) = 0;
	};
	/** Generator instance classes templated over the engine and distribution */
	template<typename DIST, typename ENGINE>
	struct Generator: public GeneratorBase
	{
		/** instantiate the generator instance from the distribution args */
		template<typename... ARGS>
		Generator(const ARGS&... args)
			: dist(args...), engine(ProperlySeededRandomEngine<ENGINE>())
		{}
		/** get the next value from this generator */
		virtual data_t next() override
		{
			auto i = dist(*engine);
			std::cerr << i << std::endl;
			return data_t{i};
		}
		/** re-seed the generator */
		virtual void seed(const data_t &seed) override
		{
			if constexpr (has_seed_method<ENGINE>::value) {
				engine->seed(get<to_data_t_type_t<typename ENGINE::result_type> >(seed));
			} else {
				throw random_error("This engine cannot be seeded");
			}
		}
		DIST dist;
		// chunks must be copiable and not all engines are
		std::shared_ptr<ENGINE> engine;
	};
	/** Enum of distributions, must match the constants in EPL */
	enum dist_t {
		DIST_UNIFORM = 0,
		DIST_NORMAL = 1,
		DIST_LOG_NORMAL = 2,
		DIST_GAMMA = 3,
		DIST_CHI_SQUARED = 4,
		DIST_CAUCHY = 5,
		DIST_FISHER_F = 6,
		DIST_STUDENT_T = 7,
		DIST_BERNOULLI = 8,
		DIST_BINOMIAL = 9,
		DIST_GEOMETRIC = 10,
		DIST_NEGATIVE_BINOMIAL = 11,
		DIST_POISSON = 12,
		DIST_EXPONENTIAL = 13,
		DIST_WEIBULL = 14,
		DIST_EXTREME_VALUE = 15,
		DIST_DISCRETE = 16
	};
	/** Enum of outputs, must match the constants in EPL */
	enum output_t {
		OUTPUT_INTEGER = 0,
		OUTPUT_REAL = 1,
		OUTPUT_BOOL = 2
	};
	/** enum of sources, must match the constants in EPL */
	enum source_t {
		SOURCE_MERSEINNE = 0,
		SOURCE_LCG = 1,
		SOURCE_DEVICE = 2
	};
public:
	Random(): base_plugin_t("Random") {}
	/** expose the methods to EPL */
	static void initialize(base_plugin_t::method_data_t &md)
	{
		md.registerMethod<decltype(&Random::createGenerator), &Random::createGenerator>("createGenerator", "action<integer, integer, integer, sequence<any> > returns chunk");
		md.registerMethod<decltype(&Random::next), &Random::next>("next", "action<chunk> returns any");
		md.registerMethod<decltype(&Random::seed), &Random::seed>("seed", "action<chunk,any>");
	}
private:
	/** construct a generator from args, given both types */
	template<typename ENGINE, typename DIST, typename... ARGS>
	custom_t<GeneratorBase> constructGeneratorDE(const ARGS&... args)
	{
		custom_t<Generator<DIST, ENGINE> > custom{new Generator<DIST, ENGINE>(args...)};
		return std::move(reinterpret_cast<custom_t<GeneratorBase>&>(custom));
	}

	/** construct a generator given the engine type with the output and distribution enums */
	template<typename ENGINE>
	custom_t<GeneratorBase> constructGeneratorE(output_t output, dist_t dist, const list_t &distArgs)
	{
		try {
			switch (dist) {
				case DIST_UNIFORM:
					switch (output) {
						case OUTPUT_INTEGER:
							return constructGeneratorDE<ENGINE, std::uniform_int_distribution<int64_t> >(get<int64_t>(distArgs[0]), get<int64_t>(distArgs[1]));
						case OUTPUT_REAL:
							return constructGeneratorDE<ENGINE, std::uniform_real_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					}
					break;
				case DIST_BERNOULLI:
					if (output == OUTPUT_BOOL)
						return constructGeneratorDE<ENGINE, std::bernoulli_distribution>(get<double>(distArgs[0]));
					break;
				case DIST_BINOMIAL:
					if (output == OUTPUT_INTEGER)
						return constructGeneratorDE<ENGINE, std::binomial_distribution<int64_t> >(get<int64_t>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_NEGATIVE_BINOMIAL:
					if (output == OUTPUT_INTEGER)
						return constructGeneratorDE<ENGINE, std::negative_binomial_distribution<int64_t> >(get<int64_t>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_GEOMETRIC:
					if (output == OUTPUT_INTEGER)
						return constructGeneratorDE<ENGINE, std::geometric_distribution<int64_t> >(get<double>(distArgs[0]));
					break;
				case DIST_POISSON:
					if (output == OUTPUT_INTEGER)
						return constructGeneratorDE<ENGINE, std::poisson_distribution<int64_t> >(get<double>(distArgs[0]));
					break;
				case DIST_EXPONENTIAL:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::exponential_distribution<double> >(get<double>(distArgs[0]));
					break;
				case DIST_GAMMA:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::gamma_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_WEIBULL:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::weibull_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_EXTREME_VALUE:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::extreme_value_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_NORMAL:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::normal_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_LOG_NORMAL:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::lognormal_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_CHI_SQUARED:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::chi_squared_distribution<double> >(get<double>(distArgs[0]));
					break;
				case DIST_CAUCHY:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::cauchy_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_FISHER_F:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::fisher_f_distribution<double> >(get<double>(distArgs[0]), get<double>(distArgs[1]));
					break;
				case DIST_STUDENT_T:
					if (output == OUTPUT_REAL)
						return constructGeneratorDE<ENGINE, std::student_t_distribution<double> >(get<double>(distArgs[0]));
					break;		
				default:
					throw random_error("Invalid distribution specified");
			}
			switch (output) {
				case OUTPUT_REAL:
				case OUTPUT_INTEGER:
				case OUTPUT_BOOL:
					throw random_error("Invalid combination of distribution and output format specified");
				default:
					throw random_error("Invalid output format specified (must be OUTPUT_REAL, OUTPUT_INTEGER or OUTPUT_BOOL)");
			}
			throw random_error("Invalid distribution, output format or combination of distribution and output format specified");
		} catch (const random_error &e) {
			throw;
		} catch (const std::exception &e) {
			throw random_error("Invalid distribution arguments specified");
		}
	}

	/** construct a generator from the enums and args */
	custom_t<GeneratorBase> constructGenerator(source_t source, output_t output, dist_t dist, const list_t &distArgs)
	{
		switch (source) {
			case SOURCE_MERSEINNE:
				return constructGeneratorE<std::mt19937_64>(output, dist, distArgs);
			case SOURCE_LCG:
				return constructGeneratorE<std::minstd_rand0>(output, dist, distArgs);
			case SOURCE_DEVICE:
				return constructGeneratorE<std::random_device>(output, dist, distArgs);
			default:
				throw random_error("Invalid random source specified (must be SOURCE_MERSEINNE, SOURCE_LCG or SOURCE_DEVICE)");
		}
	}
public:
	/** create generator method exposed to EPL. */
	custom_t<GeneratorBase> createGenerator(int64_t source, int64_t output, int64_t dist, const list_t &distArgs)
	{
		return constructGenerator((source_t)source, (output_t)output, (dist_t)dist, distArgs);
	}
	/** Next item method exposed to EPL. */
	data_t next(const custom_t<GeneratorBase> &gen)
	{
		return gen->next();
	}
	/** Seed method exposed to EPL. */
	void seed(const custom_t<GeneratorBase> &gen, const data_t &seed)
	{
		gen->seed(seed);
	}
};

APAMA_DECLARE_EPL_PLUGIN(Random)

} // com::apamax::random
