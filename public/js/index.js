
document.addEventListener("DOMContentLoaded", function () {
    
    var table = document.querySelector('table');
    var rows = Array.from(table.rows)
    
    var headers = rows[0]
    var rows = rows.slice(1)
    
    
    var nameHeader = document.querySelector('th:nth-child(1):not([colspan])');  //0
    var sizeHeader = document.querySelector('th:nth-child(2):not([colspan])');  //1
    var dateHeader = document.querySelector('th:nth-child(3):not([colspan])');  //2
    var typeHeader = document.querySelector('th:nth-child(4):not([colspan])');  //3
    var ownerHeader = document.querySelector('th:nth-child(5):not([colspan])'); //4

    var currentStatus = -1;
    var isInverse = true;

    console.log(nameHeader);
    
    //Ordenar por nombre
    nameHeader.addEventListener('click', function () { 
        rows.sort(function(a, b) {
            var aName = (a.cells[0] ? a.cells[0].textContent : '');
            var bName = (b.cells[0] ? b.cells[0].textContent : '');
            return aName.localeCompare(bName);
        });

        if(currentStatus == 0 && !isInverse) InverseSortTable();
        else SortTable();

        currentStatus = 0;
    });

    //Ordenar por tamaño
    sizeHeader.addEventListener('click', function () {
        rows.sort(function(a, b) {
            var aSize = (a.cells[1] ? a.cells[1].textContent : '');
            var bSize = (b.cells[1] ? b.cells[1].textContent : '');
            if (aSize == "-") {
                return 1;
            } else if (bSize == "-") {
                return -1;
            } else {
                var aNum = parseFloat(aSize);
                var bNum = parseFloat(bSize);

                var aFactor = getFactor(aSize);
                var bFactor = getFactor(bSize);

                return aNum*aFactor - bNum*bFactor;
            }
        });

        if(currentStatus == 1 && !isInverse) InverseSortTable();
        else SortTable();

        currentStatus = 1;
    });

    //Ordenar por fecha
    dateHeader.addEventListener('click', function () {
        rows.sort(function(a, b) {
            var aDate = (a.cells[2] ? a.cells[2].textContent : '');
            var bDate = (b.cells[2] ? b.cells[2].textContent : '');
            return aDate.localeCompare(bDate);
        });

        if(currentStatus == 2 && !isInverse) InverseSortTable();
        else SortTable();

        currentStatus = 2;
    });

    //Ordenar por tipo
    typeHeader.addEventListener('click', function () {
        rows.sort(function(a, b) {
            var aType = (a.cells[3] ? a.cells[3].textContent : '');
            var bType = (b.cells[3] ? b.cells[3].textContent : '');
            return aType.localeCompare(bType);
        });
        
        if(currentStatus == 3 && !isInverse) InverseSortTable();
        else SortTable();

        currentStatus = 3;
    });

    //Ordenar por permisos
    ownerHeader.addEventListener('click', function () {
        rows.sort(function(a, b) {
            var aOwner = (a.cells[4] ? a.cells[4].textContent : '');
            var bOwner = (b.cells[4] ? b.cells[4].textContent : '');
            return aOwner.localeCompare(bOwner);
        });

        if(currentStatus == 4 && !isInverse) InverseSortTable();
        else SortTable();

        currentStatus = 4;
    });

    function SortTable(){
        table.innerHTML = "";
        table.appendChild(headers);
        for (var i = 0; i < rows.length; i++) {
            table.appendChild(rows[i]);
        }

        isInverse = false;
    }

    function InverseSortTable(){
        table.innerHTML = "";
        table.appendChild(headers);
        for (var i = rows.length-1; i >= 0; i--) {
            table.appendChild(rows[i]);
        }

        isInverse = true;
    }
    
    function getFactor(size)
    {
        //Si es Gigabyte
        if (size.endsWith("G")) {
            return 1000000000;
        }
        //Si es Megabyte
        else if (size.endsWith("M")) {
            return 1000000;
        }
        //Si es Kilobyte
        else if (size.endsWith("K")) {
            return 1000;
        }

        return 1;
    }

});

