"""
Contains a class that trains an agent.
"""
import logging
from typing import Tuple
import numpy as np
import gym
from zmq_client import ZmqClient

from envs import make_vec_envs
from messages import (InfoMessage, MakeMessage, ResetMessage,
                      StepMessage, DynamicsMessage)

RUNNING_REWARD_HORIZON = 10


class Server(object):

    """
    When `Server.serve()` is called, provides a ZMQ based API for training
    RL agents on OpenAI gym environments.
    """

    def __init__(self, zmq_client: ZmqClient) -> None:
        self.zmq_client: ZmqClient = zmq_client
        self._env: gym.Env = None
        logging.info("Gym server initialized")

    @property
    def env(self):
        return self._env

    @env.setter
    def env(self, value):
        self._env = value

    def serve(self):
        """
        Run the server.
        """
        logging.info("Serving")
        try:
            self.__serve()
        except KeyboardInterrupt:
            pass

    def _serve(self):
        """
        Serve the request
        """

        while True:
            request = self.zmq_client.receive()
            method = request['method']
            param = request['param']

            if method == 'info':
                (action_space_type,
                 action_space_shape,
                 observation_space_type,
                 observation_space_shape,
                 observation_space_size) = self.__info()

                logging.info("Action space type " + action_space_type)
                logging.info("Action space shape " + str(action_space_shape))
                logging.info("Observation space type " + observation_space_type)
                logging.info("Observation space shape " + str(observation_space_shape))

                self.zmq_client.send(InfoMessage(action_space_type,
                                                 action_space_shape,
                                                 observation_space_type,
                                                 observation_space_shape,
                                                 observation_space_size))
            elif method == 'dynamics':
                state = param["state"]
                action = param["action"]
                data = self._env.P[state][action]
                prob = []
                next_state = []
                reward = []
                done = []

                for item in data:
                    prob.append(item[0])
                    next_state.append(item[1])
                    reward.append(item[2])
                    done.append(item[3])

                self.zmq_client.send(DynamicsMessage(prob=prob, next_state=next_state,
                                                     reward=reward, done=done))

            elif method == 'make':
                self.make(param['env_name'], param['num_envs'])
                self.zmq_client.send(MakeMessage())

            elif method == 'reset':
                observation = self.__reset()
                logging.info(" Observation " + str(observation))
                self.zmq_client.send(ResetMessage(observation))

            elif method == 'step':
                if 'render' in param:
                    result = self.__step(
                        np.array(param['actions']), param['render'])
                else:
                    result = self.__step(np.array(param['actions']))
                self.zmq_client.send(StepMessage(result[0],
                                                 result[1],
                                                 result[2],
                                                 result[3]['reward']))

    def info(self):
        """
        Return info about the currently loaded environment
        """

        action_space_type = self._env.action_space.__class__.__name__
        if action_space_type == 'Discrete':
            action_space_shape = [self._env.action_space.n]
        else:
            action_space_shape = self._env.action_space.shape
        observation_space_type = self._env.observation_space.__class__.__name__
        observation_space_shape = self._env.observation_space.shape
        observation_space_size = self._env.observation_space.n
        return (action_space_type, action_space_shape, observation_space_type,
                observation_space_shape, observation_space_size)

    def n_states(self) -> int:
        return self._env.observation_space.shape

    def make(self, env_name, num_envs):
        """
        Makes a vectorized environment of the type and number specified.
        """
        logging.info("Making %d %ss", num_envs, env_name)
        self._env = make_vec_envs(env_name, 0, num_envs)

    def reset(self) -> np.ndarray:
        """
        Resets the environments.
        """
        logging.info("Resetting environments")
        return self._env.reset()

    def step(self,
             actions: np.ndarray,
             render: bool = False) -> Tuple[np.ndarray, np.ndarray,
                                            np.ndarray, np.ndarray]:
        """
        Steps the environments.
        """
        if isinstance(self._env.action_space, gym.spaces.Discrete):
            actions = actions.squeeze(-1)
            actions = actions.astype(np.int)
        observation, reward, done, info = self._env.step(actions)
        reward = np.expand_dims(reward, -1)
        done = np.expand_dims(done, -1)
        if render:
            self._env.render()
        return observation, reward, done, info

    __info = info
    __make = make
    __reset = reset
    __serve = _serve
    __step = step
