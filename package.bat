
set OPTS=-f markdown-auto_identifiers -t html5 --section-divs -c docs/base.css

pandoc -s README.md %OPTS% -o readme.html