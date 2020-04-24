# Example 22: SARSA Learning Applied on ```CliffWorld```

## Contents

* [Overview](#overview) 
	* [SARSA Algorithm](#sarsa_algorithm)
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)


## <a name="ackw"></a>  Acknowledgements

## <a name="overview"></a> Overview

### <a name="sarsa_algorithm"></a> SARSA Algorithm
SARSA stands for State-Action-Reward-State-Action and it is an iterative algorithm
for  learning a <a href="https://en.wikipedia.org/wiki/Markov_decision_process">Markov Decision Process</a>.

As the name implies, the algorithm uses the current agent state say ```S_1```,
the action the agent chooses ```A_1```, the reward ```R``` that the agent
gets for choosing this action, the state ```S_2``` that the agent enters after
taking action ```A_1``` and the next action ```A_2``` the agent chooses when in
state ```S_2```.

Hence, the update of the ```Q``` function is given by the equation below

<img src="sarsa_update.svg"
     alt="SARSA Update"
     style="float: left; margin-right: 10px;" />

The algorithm is described below


## <a name="include_files"></a> Include files

## <a name="m_func"></a> The main function

## <a name="results"></a> Results

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>


