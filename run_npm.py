Import("env")
import subprocess
import sys

def run_npm_build(source, target, env):
    print("Running 'npm run build' for filesystem data...")
    # Change 'build' to whatever your npm script name is
    result = subprocess.run(
        "npm run build", 
        shell=True, 
        cwd="site"
    )
    
    if result.returncode != 0:
        print("Error: npm build failed!")
        sys.exit(1)

# Hook into the buildfs target before it collects the data/ folder
env.AddPreAction("buildfs", run_npm_build)
