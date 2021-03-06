#include <iostream>

#include <genie/utility/init.h>
#include <genie/execution_policy/single_value.h>
#include <genie/execution_policy/single_range.h>
#include <genie/execution_policy/validation.h>
#include <genie/exception/exception.h>

#include "execution_policy.h"

using namespace genie;
using namespace std;

shared_ptr<ExecutionPolicy> genie::MakePolicy(const Config& config)
{
	config.Validate();

	// build an ExecutionPolicy
	shared_ptr<ExecutionPolicy> generated_policy;
	if (config.IsQueryRangeSet())
	{
		execution_policy::SingleRangeExecutionPolicy* policy = new execution_policy::SingleRangeExecutionPolicy();
		policy->SetK(config.GetK());
		policy->SetNumOfQueries(config.GetNumOfQueries());
		policy->SetQueryRange(config.GetQueryRange());
		generated_policy = shared_ptr<execution_policy::SingleRangeExecutionPolicy>(policy);
	}
	else
	{
		execution_policy::SingleValueExecutionPolicy* policy = new execution_policy::SingleValueExecutionPolicy();
		policy->SetK(config.GetK());
		policy->SetNumOfQueries(config.GetNumOfQueries());
		generated_policy = shared_ptr<execution_policy::SingleValueExecutionPolicy>(policy);
	}

	// validate the policy
	if (generated_policy)
	{
		generated_policy->Validate(); // will throw exception if the configuration is invalid
		genie::utility::Init(const_cast<Config&>(config));
		return generated_policy;
	}
	else
		throw exception::InvalidConfigurationException("No execution policy is available for the configuration");
}

void genie::ExecutionPolicy::SetK(const uint32_t k)
{
	k_ = k;
}

void genie::ExecutionPolicy::SetNumOfQueries(const uint32_t num_of_queries)
{
	num_of_queries_ = num_of_queries;
}

uint32_t genie::ExecutionPolicy::GetNumOfQueries() const
{
	return num_of_queries_;
}

void genie::ExecutionPolicy::Validate()
{
	execution_policy::validation::ValidateK(k_);
	execution_policy::validation::ValidateNumOfQueries(num_of_queries_);
}
