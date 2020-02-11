#include "Finance.h"

std::map<MarketProduct, double> productionCost = {

	// Raw Materials
	{ MarketProduct::Metal, 1.0 },
	{ MarketProduct::Oil, 1.0 },
	{ MarketProduct::Coal, 1.0 },
	{ MarketProduct::Produce, 4.0 },
	{ MarketProduct::Animals, 6.0 },
	{ MarketProduct::Timber, 1.0 },

	// Products
	{ MarketProduct::Food, 3.0 },

};

const double GetProductProductionCost(MarketProduct p) {
	return productionCost[p];
}

std::map<MarketProduct, double> productionOutput = {

	// Raw Materials
	{ MarketProduct::Metal, 1.0 },
	{ MarketProduct::Oil, 1.0 },
	{ MarketProduct::Coal, 1.0 },
	{ MarketProduct::Produce, 64.0 },
	{ MarketProduct::Animals, 48.0 },
	{ MarketProduct::Timber, 10.0 },

	// Products
	{ MarketProduct::Food, 3.0 },

};

const double GetProductProductionBaseOutput(MarketProduct p) {
	return productionOutput[p];
}

std::map<MarketProduct, UInt32> conversionRate = {

	{ MarketProduct::LuxuryGoods, 1 },
	{ MarketProduct::Food, 3 },
	{ MarketProduct::Furniture, 1 },
	{ MarketProduct::Hardware, 1 },
	{ MarketProduct::Machinery, 1 },
	{ MarketProduct::Weaponry, 1 },
	{ MarketProduct::Vehicles, 1 },

};

const UInt32 GetProductConversionRate(MarketProduct p) {
	return conversionRate[p];
}
