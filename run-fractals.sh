#!/bin/bash
# Launcher script for Fractal Explorer application

echo "Starting Fractal Explorer..."
mvn clean compile exec:java -Dexec.mainClass="fractals.FractalExplorer"
