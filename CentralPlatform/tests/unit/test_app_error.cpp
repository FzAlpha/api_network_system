#include "../../Core/errors/apperrors.h"
#include <expected>
#include <iostream>
#include <string>

std::expected<std::string, CentralPlatform::Core::AppError>
fetchUserId(int userId)
{
  if (userId != 1)
  {
    return std::unexpected(CentralPlatform::Core::AppError(
        CentralPlatform::Core::ErrorType::NotFound, 404, "User not found"));
  }
  return "user_1";
}

int main()
{
  auto userResult = fetchUserId(1);
  if (userResult.has_value())
  {
    std::cout << "User found: " << userResult.value() << std::endl;
  }
  else
  {
    std::cerr << "Test failed: Expected user found, got error: " << userResult.error().message
              << " | HTTP Status: " << userResult.error().http_status
              << std::endl;
    return 1;
  }

  auto errorResult = fetchUserId(99);
  if (errorResult.has_value())
  {
    std::cerr << "Test failed: Expected error for non-existent user, but got value: " << errorResult.value() << std::endl;
    return 1;
  }
  else
  {
    std::cout << "Error handled as expected: " << errorResult.error().message
              << " | HTTP Status: " << errorResult.error().http_status
              << std::endl;
  }

  return 0;
}
