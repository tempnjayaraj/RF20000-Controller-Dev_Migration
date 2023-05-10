# Ruby file to print out the code coverage lines
# code coverage file. The code coverage files
# are present in the ./build directory. They are
# named as cc_file_Testunit.txt. We need the 
# line which says Module "unit" from each of those
# files


Dir.glob("./build/*.txt") do |my_text_file|
  mystring = "#{my_text_file}"
  substring = mystring.match(/Test([^..]*).txt$/)
  file = File.new(mystring, "r")
  while (line = file.gets)
    if line.match("Module \"" + substring[1]) then
      puts line
    end
  end
  file.close
end


