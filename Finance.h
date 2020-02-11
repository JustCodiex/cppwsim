#pragma once
#include "stdlib.h"

// A short way of representing capital (Money)
typedef long double Capital;

// A product that can be bought and sold on a market
enum class MarketProduct {
	
	// Undefined
	Undefined,

	// Materials (Primary Sector):
	Metal, // (Iron, Gold, etc.)
	Oil,
	Coal,
	Produce, // (Grain, Vegetables, Furits, etc.)
	Animals, // (Meat, Milk, Cheese, etc.)
	Timber,

	// Goods (Secondary Sector):
	LuxuryGoods,
	Food,
	Furniture,
	Hardware,
	Machinery,
	Weaponry,
	Vehicles,

	// Services (Tertiary Sector):
	Banking,
	Software,
	Transport,
	Judicial,
	Heating,

	MARKET_PRODUCT_COUNT,

};

const inline bool IsMarketMaterial(MarketProduct p) { 
	return p == MarketProduct::Metal || p == MarketProduct::Oil || p == MarketProduct::Coal || p == MarketProduct::Produce || p == MarketProduct::Animals || p == MarketProduct::Timber; 
}

const inline bool IsMarketService(MarketProduct p) {
	return p == MarketProduct::Banking || p == MarketProduct::Software || p == MarketProduct::Transport || p == MarketProduct::Judicial || p == MarketProduct::Heating;
}

const inline bool IsMarketGoods(MarketProduct p) {
	return !IsMarketMaterial(p) && !IsMarketService(p);
}

const double GetProductProductionBaseOutput(MarketProduct p);

const double GetProductProductionCost(MarketProduct p);

const UInt32 GetProductConversionRate(MarketProduct p);

const std::vector<MarketProduct> GetProductRequirements(MarketProduct p);
