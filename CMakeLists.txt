cmake_minimum_required(VERSION 3.22)

project(Prism VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# JUCE via FetchContent (pinned stable tag, verified against MSVC 14.51 / VS2026)
# ---------------------------------------------------------------------------
include(FetchContent)

set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXTRAS   OFF CACHE BOOL "" FORCE)

FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.14
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(JUCE)

# ---------------------------------------------------------------------------
# Plugin target
# ---------------------------------------------------------------------------
set(PRISM_FORMATS VST3 Standalone)
if(APPLE)
    list(APPEND PRISM_FORMATS AU)
endif()

juce_add_plugin(Prism
    COMPANY_NAME              "starfinesse"
    PRODUCT_NAME              "PRISM"
    PLUGIN_MANUFACTURER_CODE  Sfns
    PLUGIN_CODE               Prsm
    FORMATS                   ${PRISM_FORMATS}
    IS_SYNTH                  TRUE
    NEEDS_MIDI_INPUT          TRUE
    NEEDS_MIDI_OUTPUT         FALSE
    IS_MIDI_EFFECT            FALSE
    COPY_PLUGIN_AFTER_BUILD   FALSE   # avoid needing admin for C:\Program Files\Common Files\VST3
    VST3_CATEGORIES           "Instrument" "Synth")

target_sources(Prism PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/params/Parameters.cpp
    Source/params/Presets.cpp
    Source/dsp/PrismVoice.cpp
    Source/dsp/SynthEngine.cpp
    Source/dsp/Effects.cpp
    Source/ui/PrismLookAndFeel.cpp)

target_include_directories(Prism PRIVATE Source)

target_compile_definitions(Prism PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=1        # keep on until a paid JUCE seat exists
    JUCE_STRICT_REFCOUNTEDPOINTER=1)

target_link_libraries(Prism
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
