import simple_one_car_env as env
import numpy as np
import tensorflow as tf
from tensorflow import keras
import matplotlib.pyplot as plt

class ProbabilityDistribution(tf.keras.Model):
  def call(self, logits, **kwargs):
    return tf.squeeze(tf.random.categorical(logits, 1), axis=-1)


class Model(tf.keras.Model):
  def __init__(self, num_actions):
    super().__init__('mlp_policy')
    self.hidden1 = keras.layers.Dense(128, activation='relu')
    self.hidden2 = keras.layers.Dense(128, activation='relu')
    self.value = keras.layers.Dense(1, name='value')
    self.logits = keras.layers.Dense(num_actions, name='policy_logits')
    self.dist = ProbabilityDistribution()

  def call(self, inputs, **kwargs):
    x = tf.convert_to_tensor(inputs)
    hidden_logs = self.hidden1(x)
    hidden_vals = self.hidden2(x)
    return self.logits(hidden_logs), self.value(hidden_vals)

  def action_value(self, obs):
    logits, value = self.predict_on_batch(obs)
    action = self.dist.predict_on_batch(logits)
    return np.squeeze(action, axis=-1), np.squeeze(value, axis=-1)


class CarAgent:
  def __init__(self, model, lr=7e-3, gamma=0.99, value_c=0.5, entropy_c=1e-4):
    self.gamma = gamma
    self.value_c = value_c
    self.entropy_c = entropy_c

    self.model = model
    self.model.compile(optimizer=keras.optimizers.RMSprop(lr=lr),
                       loss=[self._logits_loss, self._value_loss])
  
  def choose_action(self, observation):
    action, _ = self.model.action_value(observation[None, :])
    return action

  def _returns_advantages(self, rewards, dones, values, next_value):
    returns = np.append(np.zeros_like(rewards), next_value, axis=-1)
    for t in reversed(range(rewards.shape[0])):
      returns[t] = rewards[t] + self.gamma * returns[t + 1] * (1 - dones[t])
    returns = returns[:-1]
    advantages = returns - values
    return returns, advantages

  def _value_loss(self, returns, value):
    return self.value_c * keras.losses.mean_squared_error(returns, value)

  def _logits_loss(self, actions_and_advantages, logits):
    actions, advantages = tf.split(actions_and_advantages, 2, axis=-1)
    weighted_sparse_ce = keras.losses.SparseCategoricalCrossentropy(from_logits=True)
    actions = tf.cast(actions, tf.int32)
    policy_loss = weighted_sparse_ce(actions, logits, sample_weight=advantages)
    probs = tf.nn.softmax(logits)
    entropy_loss = keras.losses.categorical_crossentropy(probs, probs)
    return policy_loss - self.entropy_c * entropy_loss


