# -*- encoding: utf-8 -*-
require File.join(File.dirname(__FILE__), "lib/marc/version")


spec = Gem::Specification.new do |s|
  s.name          = 'marc'
  s.version       = MARC::VERSION
  s.author        = 'Ed Summers'
  s.email         = 'ehs@pobox.com'
  s.homepage      = 'https://github.com/ruby-marc/ruby-marc/'
  s.platform      = Gem::Platform::RUBY
  s.summary       = 'A ruby library for working with Machine Readable Cataloging'
  s.license       = "MIT"
  s.files         = Dir.glob("{lib,test}/**/*") + Dir.glob("ext/**/*.{c,rb}") + ["Rakefile", "README.md", "Changes", "LICENSE"]
  s.require_path  = 'lib'
  s.autorequire   = 'marc'
  s.has_rdoc      = true
  s.required_ruby_version = '>= 1.8.6'
  s.authors       = ["Kevin Clarke", "Bill Dueber", "William Groppe", "Jonathan Rochkind", "Ross Singer", "Ed Summers"]
  s.test_file     = 'test/ts_marc.rb'
  s.bindir        = 'bin'

  s.extensions << "ext/readmarc/extconf.rb"
  s.add_dependency "ensure_valid_encoding"  

  s.add_dependency "unf" # unicode normalization
  s.add_development_dependency "rake-compiler"
end
