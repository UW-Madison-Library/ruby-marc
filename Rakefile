require 'rubygems'

require 'rake'
require 'rake/testtask'
require 'rake/extensiontask'
require 'rake/clean'
require 'rdoc/task'
require 'bundler/gem_tasks'


task :default => [:test]

Rake::TestTask.new('test') do |t|
  t.libs << 'lib'
  t.pattern = 'test/**/tc_*.rb'
  t.verbose = true
end


Rake::RDocTask.new('doc') do |rd|
  rd.rdoc_files.include("README", "Changes", "LICENSE", "lib/**/*.rb")
  rd.main = 'MARC::Record'
  rd.rdoc_dir = 'doc'
end

spec = Gem::Specification.load('marc.gemspec')
Rake::ExtensionTask.new do |ext|
  ext.name = 'readmarc'
  ext.ext_dir = 'ext/readmarc'
  ext.lib_dir = 'lib/marc'
  ext.gem_spec = spec
end

CLEAN.include "**/*.o", "**/*.so", "**/*.bundle", "pkg", "tmp"
