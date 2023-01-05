#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

typedef unsigned char       uchar;
typedef unsigned char       time32;

//Sistema de medición de sensores 
namespace SENSOR {

	//Localizacion de en cordenadas Xo,Yo, de la posicion de los sensores y medición. 
	struct location {
		uchar pos_x, pos_y;
	};

	class reading {

	public:
		double alpha_value(location loc, time32 t) const {
			//Me invito una ecuacion. 
			std::cout << "Alpha value " << std::endl;
			return loc.pos_x * 0.5 * t + loc.pos_y;
		}
		double beta_value(location loc, time32 t) const {
			//Me invento la ecuación. 
			std::cout << "Beta value " << std::endl;
			return loc.pos_x * 0.8 * t + loc.pos_y;
		}
	
		
	};


}//end namespace SENSOR 

void adjust_values(double* aph1, double* aph2, double* beta1, double* beta2) {
	std::cout << "Adjust values function call " << std::endl;
	*aph1 += 10;
	*aph2 += 20;
	*beta1 += 30;
	*beta2 += 40;
}


template <typename F>
struct make_tuple_of_params;

template <typename F>
struct make_tuple_of_derefed_param;


template <typename F>
using make_tuple_of_params_t		 = typename make_tuple_of_params<F>::type; //standar template when using use  a template form. 

template <typename F>
using make_tuple_of_derefed_params_t = typename make_tuple_of_derefed_param<F>::type;

template <typename Ret, typename... Args>
struct make_tuple_of_params<Ret(Args...)> {
	using type = std::tuple<Args...>;
};

template <typename Ret, typename... Args>
struct make_tuple_of_derefed_param<Ret(Args...)> {
	using type = std::tuple<std::remove_pointer_t<Args>...>;
};


//Se crea una lista con indices para recorrer y de esta forma, aplicar la funcion con los párametros dados.
//Esta forma realiza una compilación mucho mayor que la recursión o Macros. 
//Se debe iterar en cada funcion y de esa funcion aplicar los parametros correspondientos iterados. 
template <std::size_t FunctionIndex,
		  typename FunctionsTuple, 
		  typename Params, 
		  std::size_t... I>
auto dispatch_params(FunctionsTuple& f, Params& params, std::index_sequence<I...>) {
	auto var = (std::get<FunctionIndex>(f))(std::get<I>(params)...);
	return var;
}



//Ejecutar los dispatch_params para agruparlo en la tupla de funciones para ser llamados. 
template<typename FunctionsTuple,
		 typename std::size_t... I,
		 typename Params,
		 typename ParamsSeq>
auto dispatch_functions(FunctionsTuple& f, 
						std::index_sequence<I...>, 
						Params& params, 
						ParamsSeq params_seq) 
{
	return std::make_tuple(dispatch_params<I>(f, params, params_seq)...);
}

template <typename F, typename Tuple, std::size_t... I>
void dispatch_to_c(F f, Tuple& t, std::index_sequence<I...>)
{
	f(&std::get<I>(t)...);
}

//Lo correcto es decirle al compilador la lista de funciones y de parametros, pero al ser una lista de argumentos
//Es necesario identificar donde empieza una y donde acaba la otra. 
template <typename LegacyFunction,
		  typename... Functions,
	      typename... Params>
auto magic_wand(
	LegacyFunction legacy,
	const std::tuple<Functions...>& f,
	const std::tuple<Params...>&   p1,
	const std::tuple<Params...>& p2)
{
	static const std::size_t functions_count = sizeof...(Functions);
	static const std::size_t params_count = sizeof...(Params);
		
	 auto params =  std::tuple_cat(
			dispatch_functions(f,
				std::make_index_sequence<functions_count>(),
				p1,
				std::make_index_sequence<params_count>()),
			dispatch_functions(f,
				std::make_index_sequence<functions_count>(),
				p2,
				std::make_index_sequence<params_count>())
		);
	 
	 static const auto t_count = std::tuple_size<decltype(params)>::value;
	 dispatch_to_c(legacy, params, std::make_index_sequence<t_count>());
	 return params;
}

//Imaginemos que tenemos varias funciones o clases de diferente lectura y queremos generalizar sus metodos. 
//De tal forma que no necesitemos, estar continuamente modificando y hacerlo lo mas veratil posible
//Usaremos las lambdas, para pasar los valores, y las funciones que tenemos de la formam as general posible. 
template <typename Reading,typename LegacyFunction>
std::tuple<double, double, double, double> get_adjusted_values(Reading& r,LegacyFunction f,
	SENSOR::location l,
	time32 t1,
	time32 t2) {

	return magic_wand(f,
		std::make_tuple(
			[&r](SENSOR::location l, time32 t) {return r.beta_value(l, t); },
			[&r](SENSOR::location l, time32 t) {return r.alpha_value(l, t); }
			),
		std::make_tuple(l, t1),
		std::make_tuple(l, t2));
}

int main() {

	SENSOR::reading ReadingTemperature;
	
	//Ejemplo. 
	auto result = get_adjusted_values(ReadingTemperature,adjust_values, { 10,10 }, 12.0, 12.0);
	

	return 0;
}