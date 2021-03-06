# Utility functions

require 'git'

LANE_APPLE_IDS = {
    'ios alpha' => '981253550',
    'ios beta' => '974795031',
    'ios store' => '324715238'
}.freeze

def export_apple_id(lane)
  ENV['WMF_APPLE_ID'] = LANE_APPLE_IDS[lane]
  puts "Exported apple_id '#{ENV['WMF_APPLE_ID']}' for delivery from lane '#{lane}'."
end

# Returns true if the `NO_DEPLOY` env var is set to 1
def deploy_disabled?
  ENV['NO_DEPLOY'] == '1'
end

# Returns true if the `NO_TEST` env var is set to 1
def test_disabled?
  ENV['NO_TEST'] == '1'
end

# Runs goals from the project's Makefile, this requires going up to the project directory.
# :args: Additional arguments to be passed to `make`.
# Returns The result of the `make` command
def make(args)
  # Maybe we should write an "uncrustify" fastlane action?...
  Dir.chdir '..' do
    sh 'make ' + args
  end
end

# Generate a list of commit subjects from `rev` to `HEAD`
# :rev: The git SHA to start the log from, defaults to `ENV[LAST_SUCCESS_REV']`
def generate_git_commit_log(rev=ENV['GIT_PREVIOUS_SUCCESSFUL_COMMIT'] || 'HEAD^^^^^')
  g = Git.open ENV['PWD']
  begin
    change_log = g.log.between(rev).map { |c| "- " + c.message.lines.first.chomp }.join "\n"
    "Commit Log:\n\n#{change_log}\n"
  rescue
    "Unable to parse commit logs"
  end
end

# Memoized version of `generate_git_commit_log` which stores the result in `ENV['GIT_COMMIT_LOG']`.
def git_commit_log
  ENV['GIT_COMMIT_LOG'] || ENV['GIT_COMMIT_LOG'] = generate_git_commit_log
end

# Parses JSON output of `plutil`
def info_plist_to_hash(path)
  require 'json'
  JSON.parse! %x[plutil -convert json -o - #{path}]
end

