HERE = File.expand_path(File.dirname(__FILE__)) + '/'

require 'rake'
require 'rake/clean'
require 'rake/testtask'
require './rakefile_helper'
require './rakefile_helper2'

include RakefileHelpers

CLOBBER.include(HERE+'build/'+'*.*')
CLOBBER.include(HERE+'mocks/'+'*.*')

#$stdout.reopen("out.log", "w")
#$stdout.sync = true
#$stderr.reopen($stdout)

# Load default configuration, for now
DEFAULT_CONFIG_FILE = 'iar_v90.yml'
configure_toolchain(DEFAULT_CONFIG_FILE)

task :unit do
  run_tests(get_unit_test_files)
end

task :unit_bulls do
  run_tests_bulls(get_unit_test_files)
end


desc "Generate test summary"
task :summary do
  report_summary
end



desc "Build and test Unity"
task :all => [:clean, :unit_bulls,:unit, :summary]
task :default => [:clobber, :all]
task :ci => [:default]
task :cruise => [:default]



desc "Load configuration"
task :config, :config_file do |t, args|
  configure_toolchain(args[:config_file])
end
