FETCHCONTENT_DECLARE(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

#you can add the library to your target by using TARGET_LINK_LIBRARY(YOUR_TARGET PUBLIC nlohmann_json)

FETCHCONTENT_MAKEAVAILABLE(json)