"""
Classes for building requests to send to pytorch-cpp-rl.
"""
from abc import ABC, abstractmethod
import numpy as np
import msgpack
import logging


class Message(ABC):
    """
    Base class for messages.
    """
    @abstractmethod
    def to_msg(self) -> bytes:
        """
        Creates the JSON for the request.
        """


class InfoMessage(Message):
    """
    Builds the JSON for returning the result of an info() action.
    """

    def __init__(self, action_space_type, action_space_shape,
                 observation_space_type, observation_space_shape,
                 observation_space_size):
        self.action_space_type = action_space_type
        self.action_space_shape = action_space_shape
        self.observation_space_type = observation_space_type
        self.observation_space_shape = observation_space_shape
        self.observation_space_size = observation_space_size

    def to_msg(self) -> bytes:
        request = {
            "action_space_type": self.action_space_type,
            "action_space_shape": self.action_space_shape,
            "observation_space_type": self.observation_space_type,
            "observation_space_shape": self.observation_space_shape,
            "observation_space_size": self.observation_space_size
        }
        return msgpack.packb(request)


class DynamicsMessage(Message):
    """
    Builds the JSON for returning the result of dynamics request.
    """

    def __init__(self, prob: float, next_state: int,
                 reward: float, done: bool) -> None:
        self.prob = prob
        self.next_state = next_state
        self.reward = reward
        self.done = done

    def to_msg(self) -> bytes:
        request = {
            "probability": self.prob,
            "next_state": self.next_state,
            "reward": self.reward,
            "done": self.done,
        }
        return msgpack.packb(request)


class MakeMessage(Message):
    """
    Builds the JSON for returning the result of an make_env() action.
    """

    def to_msg(self) -> bytes:
        request = {
            "result": "OK"
        }
        return msgpack.packb(request)


class ResetMessage(Message):
    """
    Builds the JSON for returning the result of an env.reset() action.
    """

    def __init__(self, observation: np.ndarray):
        self.observation = observation

    def to_msg(self) -> bytes:

        pack_obs = []
        if isinstance(self.observation, list):
            for item in self.observation:
                try:
                    pack_obs.append(item.tolist())
                except:
                    pack_obs.append(item)

        request = {
            "observation": pack_obs # self.observation.tolist()
        }
        return msgpack.packb(request)


class StepMessage(Message):
    """
    Builds the JSON for returning the result of an env.step() action.
    """

    def __init__(self,
                 observation: np.ndarray,
                 reward: np.ndarray,
                 done: np.ndarray,
                 real_reward: np.ndarray):
        self.observation = observation
        self.reward = reward
        self.done = done
        self.real_reward = real_reward

    def to_msg(self) -> bytes:
        request = {
            "observation": self.observation.tolist(),
            "reward": self.reward.tolist(),
            "done": self.done.tolist(),
            "real_reward": self.real_reward.tolist()
        }
        return msgpack.packb(request)
