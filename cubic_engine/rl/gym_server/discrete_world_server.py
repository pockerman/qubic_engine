import logging

from zmq_client import ZmqClient
import gym
from server import Server


class DiscreteWorldServer(Server):

    def __init__(self, zmq_client: ZmqClient) -> None:
        super(DiscreteWorldServer, self).__init__(zmq_client=zmq_client)

    def make(self, env_name, num_envs=1):
        """
        Makes a vectorized environment of the type and number specified.
        """
        logging.info("Making %d %ss", num_envs, env_name)
        self.env = gym.make(env_name)
