-- bug fix the update/insert trigger of Civs table
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


ALTER   Trigger Civs_Trigger_InsUpd
On dbo.Civs
For Insert, Update
As
If Update (TechBitsCiv)
Begin
    declare @ok bit
    declare @techbits char(800)
    select @ok = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsCiv From Inserted
    Open curIns

    Fetch Next From curIns Into @techbits
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbits, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsCiv', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbits
    End

    Close curIns
End
If Update (TechBitsCivNoDev)
Begin
    declare @okNoDev bit
    declare @techbitsNoDev char(800)
    select @okNoDev = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns2 Cursor Local For
    	Select TechBitsCivNoDev From Inserted
    Open curIns2

    Fetch Next From curIns2 Into @techbitsNoDev
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsNoDev, @okNoDev Output
    	If @okNoDev=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsCivNoDev', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns2 Into @techbitsNoDev
    End

    Close curIns2
End

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-- end of bugfix


-- GT
INSERT INTO [Federation].[dbo].[Civs]
([CivID], [Name], [Description], [EscapePodShipTypeID], [TechBitsCiv], [StartingGlobalAttributeID], [InitialStationTypeID], [IconName], [StationHUD], [TechBitsCivNoDev], [BonusMoney], [IncomeMoney])
VALUES(3, 'Ga''Taraan', 'Ga''Taraan', 62, '', 6, 30, 'gataaran', 'dialog', '', 0, 0)

INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(0, 3,0, 'Private')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(1, 3,500, 'Sergeant')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(2, 3,2000, 'Captain')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(3, 3,5000, 'Lieutenant')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(4, 3,9000, 'Major')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(5, 3,14000, 'Colonel')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(6, 3,20000, 'Guardsman')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(7, 3,26000, 'Elite Guardsman')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(8, 3,34000, 'Guard Commander')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(9, 3,43000, 'Commander')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(10, 3,54000, 'Commodore')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(11, 3,65000, 'Battlegroup Commander')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(12, 3,77000, 'Fleet Commander')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(13, 3,90000, 'Admiral')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(14, 3,105000, 'High Admiral')

-- end of GT

-- Dreg
INSERT INTO [Federation].[dbo].[Civs]
([CivID], [Name], [Description], [EscapePodShipTypeID], [TechBitsCiv], [StartingGlobalAttributeID], [InitialStationTypeID], [IconName], [StationHUD], [TechBitsCivNoDev], [BonusMoney], [IncomeMoney])
VALUES(1, 'Dreghklar', 'Dreghklar', 33, '', 7, 26, 'lizards', 'dialog', '', 0.25, 0.25)

INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(0, 1,0, 'Podling')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(1, 1,500, 'Tadpole')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(2, 1,2000, 'Citizen')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(3, 1,5000, 'Auxiliary')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(4, 1,9000, 'Legionary')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(5, 1,14000, 'Optio')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(6, 1,20000, 'Decurion')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(7, 1,26000, 'Centurion')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(8, 1,34000, 'Legion Commander')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(9, 1,43000, 'Tribune')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(10, 1,54000, 'Legate')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(11, 1,65000, 'Count')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(12, 1,77000, 'Duke')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(13, 1,90000, 'Overlord')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(14, 1,105000, 'Imperator ')

-- end of Dreg

-- TF
INSERT INTO [Federation].[dbo].[Civs]
([CivID], [Name], [Description], [EscapePodShipTypeID], [TechBitsCiv], [StartingGlobalAttributeID], [InitialStationTypeID], [IconName], [StationHUD], [TechBitsCivNoDev], [BonusMoney], [IncomeMoney])
VALUES(2, 'Technoflux', 'Technoflux', 64, '', 8, 40, 'technoflux', 'dialog', '', -0.2, -0.166)

INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(0, 2,0, 'Bit')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(1, 2,500, 'Byte')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(2, 2,2000, 'Word')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(3, 2,5000, 'Register')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(4, 2,9000, 'Index')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(5, 2,14000, 'Variable')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(6, 2,20000, 'Subroutine')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(7, 2,26000, 'Program')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(8, 2,34000, 'Utility')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(9, 2,43000, 'Application')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(10, 2,54000, 'Compiler')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(11, 2,65000, 'Driver')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(12, 2,77000, 'Daemon')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(13, 2,90000, 'Kernel')
INSERT INTO [Federation].[dbo].[Ranks]([Rank], [CivID], [Requirement], [Name]) VALUES(14, 2,105000, 'OS')

--- end of TF

-- fix little bug in profile

use dbo.profile
go

ALTER PROCEDURE dbo.p_edit_team_profile_aleg
	(
		@SquadID int,
		@Description varchar(510),
		@URL varchar(255),
		@ErrorCode int output,
		@ErrorMsg varchar(128) output
	)
AS
	SET NOCOUNT ON
	select @ErrorCode = 0
	select @ErrorMsg = ''

	/* check if squad exist */
	declare @rc int
	select @rc = count(*) from Squads where SquadID = @SquadID
	if @rc <> 1
	begin
		select @ErrorCode = 1
		select @ErrorMsg = 'squad doesnt exist'
		return
	end
	update Squads set Description=@Description, URL=@URL
	 
	RETURN 
go