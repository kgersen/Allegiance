
declare @iLastBuild int
declare @iBuild     int

select
    @iLastBuild = max(passid)
from
    passes
where
    (passid < 99999)

insert into
    passes
values(
    BUILDNUM * 10,
    'Build ' + convert(varchar, BUILDNUM),
    2,
    0,
    1,
	TODAYSDATE,
	TODAYSDATE,
	TODAYSDATE)

insert into
    passareaowners
    select BUILDNUM * 10, AreaID, TesterID
        from
            passareaowners
        where
            (PassID = @iLastBuild)


