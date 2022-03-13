#pragma once

#include <vector>
#include <algorithm>
#include <utility>

enum EmotionalAxis
{
	JoySadness,
	TrustDisgust,
	FearAnger,
	SurpriseAnticipation,
	NumEmotionalAxis
};

enum PersonalityAxis
{
	Openness,
	Conscientiousness,
	Extroversion,
	Agreeableness,
	Neuroticism,
	NumPersonalityAxis
};

struct EmotionalState
{
	std::vector<float> m_emotionalAxis;

	EmotionalState(float js, float td, float fa, float sa)
	{
		m_emotionalAxis.resize(NumEmotionalAxis);

		m_emotionalAxis[JoySadness] = js;
		m_emotionalAxis[TrustDisgust] = td;
		m_emotionalAxis[FearAnger] = fa;
		m_emotionalAxis[SurpriseAnticipation] = sa;
	}

	EmotionalState()
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis.push_back(((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2) - 1);
	}

	EmotionalState(const EmotionalState& state)
	{
		m_emotionalAxis = state.m_emotionalAxis;
	}

	float& operator[](int i)
	{
		if (i < 0 || i > NumEmotionalAxis)
			return m_emotionalAxis[0];

		return m_emotionalAxis[i];
	}

	EmotionalState operator+=(EmotionalState& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] += other.m_emotionalAxis[i];

		return *this;
	}

	EmotionalState operator-=(EmotionalState& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] -= other.m_emotionalAxis[i];

		return *this;
	}

	EmotionalState operator*=(EmotionalState& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] *= other.m_emotionalAxis[i];

		return *this;
	}

	EmotionalState operator/=(EmotionalState& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] /= other.m_emotionalAxis[i];

		return *this;
	}

	EmotionalState operator+(EmotionalState& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] + other.m_emotionalAxis[i];

		return state;
	}

	EmotionalState operator-(EmotionalState& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] - other.m_emotionalAxis[i];

		return state;
	}

	EmotionalState operator*(EmotionalState& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] * other.m_emotionalAxis[i];

		return state;
	}

	EmotionalState operator/(EmotionalState& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] / other.m_emotionalAxis[i];

		return state;
	}

	EmotionalState operator+=(float& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] += other;

		return *this;
	}

	EmotionalState operator-=(float& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] -= other;

		return *this;
	}

	EmotionalState operator*=(float& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] *= other;

		return *this;
	}

	EmotionalState operator/=(float& other)
	{
		for (int i = 0; i < NumEmotionalAxis; i++)
			m_emotionalAxis[i] /= other;

		return *this;
	}

	EmotionalState operator+(float& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] + other;

		return state;
	}

	EmotionalState operator-(float& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] - other;

		return state;
	}

	EmotionalState operator*(float& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] * other;

		return state;
	}

	EmotionalState operator/(float& other)
	{
		EmotionalState state;
		for (int i = 0; i < NumEmotionalAxis; i++)
			state[i] = m_emotionalAxis[i] / other;

		return state;
	}

	float magSquared()
	{
		float v = 0.0f;
		for (float e : m_emotionalAxis)
		{
			v += e*e;
		}
		return v;
	}

	float mag()
	{
		return sqrt(magSquared());
	}
};

struct Personality
{
	std::vector<float> m_personalityAxis;

	Personality(float o, float c, float e, float a, float n)
	{
		m_personalityAxis.resize(NumPersonalityAxis);

		m_personalityAxis[Openness] = o;
		m_personalityAxis[Conscientiousness] = c;
		m_personalityAxis[Extroversion] = e;
		m_personalityAxis[Agreeableness] = a;
		m_personalityAxis[Neuroticism] = n;
	}

	float GetAxis(int axis)
	{
		if (axis < 0 || axis > NumPersonalityAxis)
			return -1.0f;

		return m_personalityAxis[axis];
	}
};

enum Moods
{
	// Primary axis moods
	Joy,
	Trust,
	Fear,
	Surprise,
	Sadness,
	Disgust,
	Anger,
	Anticipation,

	// Primary Dyad
	Love,
	Submission,
	Alarm,
	Disappointment,
	Remorse,
	Contempt,
	Aggression,
	Optimism,

	// Secondary Dyad
	Guilt,
	Curiousity,
	Despair,
	Envy,
	Cynicism,
	Pride,
	Fatalism,

	// Tertiary Dyad
	Delight,
	Sentimentality,
	Shame,
	Outrage,
	Pessimism,
	Morbidness,
	Dominance,
	Anxiety,

	NumMoods
};

static EmotionalState g_fMoodValues[NumMoods] =
{
	// Emotional axes
	EmotionalState(1, 0, 0, 0), // Joy
	EmotionalState(0, 1, 0, 0), // Trust
	EmotionalState(0, 0, 1, 0), // Fear
	EmotionalState(0, 0, 0, 1), // Surprise
	EmotionalState(-1, 0, 0, 0), // Sadness
	EmotionalState(0, -1, 0, 0), // Disgust
	EmotionalState(0, 0, 0, -1), // Anger
	EmotionalState(0, 0, -1, 0), // Anticipation

	// Primary Dyads
	EmotionalState(1, 1, 0, 0), // Love
	EmotionalState(0, 1, 1, 0), // Submission
	EmotionalState(0, 0, 1, 1), // Alarm
	EmotionalState(-1, 0, 1, 0), // Disappointment
	EmotionalState(-1, 1, 0, 0), // Remorse
	EmotionalState(0, -1, 0, -1), // Contempt
	EmotionalState(0, 0, -1, -1), // Aggression
	EmotionalState(1, 0, -1, 0), // Optimism

	// Secondary Dyads
	EmotionalState(1, 0, 0, 1), // Guilt
	EmotionalState(0, 1, 1, 0), // Curiousity
	EmotionalState(-1, 0, 0, 1), // Despair
	EmotionalState(-1, 0, 0, -1), // Envy
	EmotionalState(0, -1, 1, 0), // Cynicism
	EmotionalState(1, 0, 0, -1), // Pride
	EmotionalState(0, 1, -1, 0), // Fatalism

	// Tertiary Dyads
	EmotionalState(1, 0, 1, 0), // Delight
	EmotionalState(-1, 1, 0, 0), // Sentimentality
	EmotionalState(0, -1, 0, 1), // Shame
	EmotionalState(0, 0, 1, -1), // Outrage
	EmotionalState(-1, 0, -1, 0), // Pessimism
	EmotionalState(1, -1, -0, 0), // Morbidness
	EmotionalState(0, 1, 0, -1), // Dominance
	EmotionalState(0, 0, -1, 1), // Anxiety
};

static std::string g_sMoodValues[NumMoods] =
{
	// Emotional axes
	"Joy",
	"Trust",
	"Fear",
	"Surprise",
	"Sadness",
	"Disgust",
	"Anger",
	"Anticipation",

	// Primary Dyad
	"Love",
	"Submission",
	"Alarm",
	"Disappointment",
	"Remorse",
	"Contempt",
	"Aggression",
	"Optimism",

	// Secondary Dyad
	"Guilt",
	"Curiousity",
	"Despair",
	"Envy",
	"Cynicism",
	"Pride",
	"Fatalism",

	// Tertiary Dyad
	"Delight",
	"Sentimentality",
	"Shame",
	"Outrage",
	"Pessimism",
	"Morbidness",
	"Dominance",
	"Anxiety"
};

// These matrices bias stimuli application
static float g_PersonalityBiasesPositive[NumEmotionalAxis][NumPersonalityAxis] =
{
	// O  C  E  A  N
	{  0, 0, 0, 0, 0  },  // Joy
	{  0, 0, 0, 0, 0  },  // Trust
	{  0, 0, 0, 0, 0  },  // Fear
	{  0, 0, 0, 0, 0  },  // Surprise
};

static float g_PersonalityBiasesNegative[NumEmotionalAxis][NumPersonalityAxis] =
{
	// O  C  E  A  N
	{  0, 0, 0, 0, 0  },  // Sadness
	{  0, 0, 0, 0, 0  },  // Disgust
	{  0, 0, 0, 0, 0  },  // Anger
	{  0, 0, 0, 0, 0  },  // Anticipation
};

// This matrix helps form the baseline personalities
static float g_BaselineEmotionalStateBiases[NumEmotionalAxis][NumPersonalityAxis] =
{
	// O  C  E  A  N
	{  0.1, 0, 0, 0, -0.2  },  // Joy/Sadness
	{  0.3, -0.25, 0, 0.3, 0  },  // Trust/Disgust
	{  0, 0.1, 0, 0, 0.25  },  // Fear/Anger
	{  0, 0, 0, 0, -0.4  },  // Surprise/Anticipation
};

static float clip(float value, float lower, float upper)
{
	return std::max(lower, std::min(value, upper));
}

static EmotionalState GetBaselineEmotionalState(Personality p)
{
	EmotionalState state = EmotionalState(0, 0, 0, 0);

	for (int i = 0; i < NumEmotionalAxis; i++)
	{
		for (int j = 0; j < NumPersonalityAxis; j++)
		{
			state[i] += g_BaselineEmotionalStateBiases[i][j] * p.GetAxis(j);
		}
	}
	
	for (int i = 0; i < NumEmotionalAxis; i++)
		state.m_emotionalAxis[i] = clip(state.m_emotionalAxis[i], -1, 1);
	
	return state;
}

static std::pair<EmotionalState, std::string> IdentifyEmotionalState(EmotionalState& state)
{
	int lowestValueFound = -1;
	float lowestDistanceFound = FLT_MAX;

	for (int i = 0; i < NumMoods; i++)
	{
		EmotionalState dif = g_fMoodValues[i] - state;
		float mag = dif.mag();
		if (mag < lowestDistanceFound)
		{
			lowestValueFound = i;
			lowestDistanceFound = mag;
		}
	}

	if (lowestValueFound != -1)
		return std::pair<EmotionalState, std::string>(g_fMoodValues[lowestValueFound], g_sMoodValues[lowestValueFound]);
}