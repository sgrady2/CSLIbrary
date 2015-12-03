require 'wavefile'
include WaveFile    # To avoid prefixing classes with 'WaveFile::'

FILES_TO_APPEND = ["one.wav", "two.wav", "three.wav"]
SAMPLE_FRAMES_PER_BUFFER = 4096
OUTPUT_FORMAT = Format.new(:stereo, :pcm_16, 44100)

Writer.new("one_two_three.wav", OUTPUT_FORMAT) do |writer|
  FILES_TO_APPEND.each do |file_name|
    Reader.new(file_name).each_buffer(SAMPLE_FRAMES_PER_BUFFER) do |buffer|
      writer.write(buffer)
    end
  end
end
