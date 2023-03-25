# If your app depends the VST2 SDK, perhaps to host VST2 plugins, CMake needs to be told where
# to find the SDK on your system. This setup should be done before calling `juce_add_gui_app`.

# juce_set_vst2_sdk_path(...)

juce_add_gui_app(PinkTromboneApp
  # VERSION ...                       # Set this if the app version is different to the project version
  # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
  # ICON_SMALL ...
  # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
  PLUGIN_NAME "Pink Trombone"
  COMPANY_NAME "cutelabnyc"           # Specify the name of the app's author
  PRODUCT_NAME "Pink Trombone")       # The name of the final executable, which can differ from the target name

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

juce_generate_juce_header(PinkTromboneApp)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources(PinkTromboneApp
  PRIVATE
  src/Main.cpp
  src/MainComponent.cpp
  src/ADSRUI.cpp
  src/Biquad.cpp
  src/EnvelopeComponent.cpp
  src/Glottis.cpp
  src/HiddenLookAndFeel.cpp
  src/LFOComponent.cpp
  src/ModLookAndFeel.cpp
  src/ModSliderComponent.cpp
  src/Modulatable.cpp
  src/PinkTromboneADSR.cpp
  src/PluginEditor.cpp
  src/PluginProcessor.cpp
  src/SimpleLFO.cpp
  src/SimplexNoise.cpp
  src/Tract.cpp
  src/TractUI.cpp
  src/WhiteNoise.cpp
  )

include_directories(
  "src"
  )

target_compile_definitions(PinkTromboneApp

  PRIVATE
  # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
  JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
  JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
  JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:PinkTromboneApp,JUCE_PRODUCT_NAME>"
  JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:PinkTromboneApp,JUCE_VERSION>")

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_gui_extra` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.
target_link_libraries(PinkTromboneApp
  PRIVATE
  juce::juce_gui_extra
  juce::juce_audio_basics
  juce::juce_audio_devices
  juce::juce_audio_formats
  juce::juce_audio_processors
  juce::juce_audio_utils
  juce::juce_data_structures
  juce::juce_dsp
  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags)
