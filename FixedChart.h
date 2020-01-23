#pragma once
#include <vector>
#include <stdexcept>

#define float_is_near(f, v) (f >= v-0.025f && f <= v+0.025f) 

struct FixedChart {

	FixedChart(){}
	FixedChart(std::vector<float> values) {
		m_values = values;
		FixUp();
	}
	FixedChart(float values[], int size) {
		for (int i = 0; i < size; i++) {
			m_values.push_back(values[i]);
		}
		FixUp();
	}

	virtual void Add(float value) {
		m_values.push_back(value);
		FixUp();
	}

	virtual void Update(int index, float value) {
		m_values[index] += value;
		FixUp();
	}

	virtual float At(int index){
		if (index >= 0 && index < (int)m_values.size()) {
			return m_values[index];
		} else {
			throw std::out_of_range("Index out of bounds.");
		}
	}

protected:

	virtual float Sum() {

		float sum = 0.0f;

		for (float f : m_values) {
			sum += f;
		}

		return sum;

	}

	virtual void FixUp() {
		
		float sum = Sum();

		if (!float_is_near(sum, 1.0f)) {

			if (sum > 1.0f) {
				FixLarge(sum);
			} else {
				FixSmall(sum);
			}

		}

	}

	virtual void FixLarge(float sum) {

		float leftover = sum - 1.0f;
		float cutEqual = leftover / m_values.size();

		for (size_t i = 0; i < m_values.size(); i++) {
			if (m_values[i] - cutEqual < 0.0f) {
				m_values[i] = 0.0f;
			} else {
				m_values[i] -= cutEqual;
			}
		}

		FixUp();

	}

	virtual void FixSmall(float sum) {

		float lacking = 1.0f - sum;
		float gainEqual = lacking / m_values.size();

		for (size_t i = 0; i < m_values.size(); i++) {
			if (m_values[i] + gainEqual > 1.0f) {
				m_values[i] = 1.0f;
			} else {
				m_values[i] += gainEqual;
			}
		}

		FixUp();

	}

protected:
	std::vector<float> m_values;

};
