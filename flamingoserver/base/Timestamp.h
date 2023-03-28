#pragma once

#include<stdint.h>
#include<algorithm>
#include<string>

using namespace std;
class Timestamp {
public:
	Timestamp(): microSecondSinceEpoch_(0){}
	explicit Timestamp(int64_t microSecondSinceEpoch);

	Timestamp& operator+=(Timestamp lhs) {
		this->microSecondSinceEpoch_ += lhs.microSecondSinceEpoch_;
		return *this;
	}

	Timestamp& operator+=(int64_t lhs) {
		this->microSecondSinceEpoch_ += lhs;
		return *this;
	}

	Timestamp& operator-=(Timestamp lhs) {
		this->microSecondSinceEpoch_ -= lhs.microSecondSinceEpoch_;
		return *this;
	}

	Timestamp& operator-=(int64_t lhs) {
		this->microSecondSinceEpoch_ -= lhs;
		return *this;
	}

	void swap(Timestamp& that) {
		std::swap(microSecondSinceEpoch_, that.microSecondSinceEpoch_);
	}

	string toString() const;
	string toFormattedString(bool showMicroseconds = true) const;

	bool valid() const { return microSecondSinceEpoch_ > 0; }

	int64_t microSecondSinceEpoch_() const { return microSecondSinceEpoch_; }

	time_t secondsSinceEpoch() const {
		return static_cast<time_t>(microSecondSinceEpoch_ / kMicroSecondsPerSecond);
	}

	static Timestamp now();
	static Timestamp invalid();

	static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
	int64_t   microSecondSinceEpoch_;
};