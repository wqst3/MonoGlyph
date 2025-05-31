#pragma once

/**
 * @brief Enum class representing unique identifiers for different states.
 *
 * Each value corresponds to a specific state that the application can be in.
 */
enum class StateID {
  Menu,     /**< State for the main menu. */
  Infinite, /**< State representing infinite mode. */
  Restart   /**< State for restarting. */
};
